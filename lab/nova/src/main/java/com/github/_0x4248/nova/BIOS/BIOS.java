package com.github._0x4248.nova.BIOS;

import com.github._0x4248.nova.BIOS.machines.Machine;
import com.github._0x4248.nova.BIOS.machines.StandardMachine;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.net.URL;
import java.net.URLClassLoader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.jar.Attributes;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

public class BIOS {
	private static volatile BiosRuntime activeRuntime;
	private static final int LOG_FOREGROUND = 15;
	private static final int LOG_BACKGROUND = 1;
	private static final int HEADER_FOREGROUND = 14;
	private static final DateTimeFormatter CLOCK_FORMAT = DateTimeFormatter.ofPattern("HH:mm:ss");

	private final Machine machine;
	private final BiosRuntime runtime;
	private final List<String> logLines;
	private final int maxLogLines;

	public BIOS() {
		this(new StandardMachine());
	}

	public BIOS(Machine machine) {
		this.machine = machine;
		this.runtime = new BiosRuntime(machine);
		activeRuntime = this.runtime;
		this.logLines = new ArrayList<>();
		this.maxLogLines = 22;

		initializeScreen();
	}

	public static BiosRuntime getRuntime() {
		return activeRuntime;
	}

	public boolean bootApplication(Path applicationJarPath, String[] applicationArgs) {
		if (applicationJarPath == null) {
			log("Boot failed: no application path provided.");
			return false;
		}

		if (!Files.exists(applicationJarPath) || !Files.isRegularFile(applicationJarPath)) {
			log("Boot failed: application not found at " + applicationJarPath.toAbsolutePath());
			return false;
		}

		log("Boot device detected: " + applicationJarPath.getFileName());

		String mainClassName;
		try {
			mainClassName = resolveMainClass(applicationJarPath);
		} catch (IOException e) {
			log("Boot failed: cannot read application jar metadata.");
			return false;
		}

		if (mainClassName == null || mainClassName.isBlank()) {
			log("Boot failed: no boot entry found (manifest Main-Class, Boot, or Main).");
			return false;
		}

		log("Entrypoint: " + mainClassName);
		log("Handing off control to application...");
		runtime.beep(70, 720);

		URL jarUrl;
		try {
			jarUrl = applicationJarPath.toUri().toURL();
		} catch (IOException e) {
			log("Boot failed: invalid application jar URL.");
			return false;
		}

		try (URLClassLoader classLoader = new URLClassLoader(new URL[]{jarUrl}, BIOS.class.getClassLoader())) {
			invokeMainClass(mainClassName, classLoader, applicationArgs);
			System.out.println("[BIOS] Application exited.");
			return true;
		} catch (ClassNotFoundException e) {
			log("Boot failed: main class not found: " + mainClassName);
			return false;
		} catch (NoSuchMethodException e) {
			log("Boot failed: class has no main(String[] args): " + mainClassName);
			return false;
		} catch (IllegalAccessException e) {
			log("Boot failed: cannot access main method: " + mainClassName);
			return false;
		} catch (InvocationTargetException e) {
			log("Application crashed: " + e.getTargetException());
			return false;
		} catch (IOException e) {
			log("Boot failed: cannot open classloader.");
			return false;
		}
	}

	public boolean bootInternalApplication(String className, String[] applicationArgs) {
		if (className == null || className.isBlank()) {
			log("Boot failed: no internal class name provided.");
			return false;
		}

		log("Boot source: internal ROM application");
		log("Entrypoint: " + className);
		log("Handing off control to application...");
		runtime.beep(70, 720);

		try {
			invokeMainClass(className, BIOS.class.getClassLoader(), applicationArgs);
			System.out.println("[BIOS] Application exited.");
			return true;
		} catch (ClassNotFoundException e) {
			log("Boot failed: main class not found: " + className);
			return false;
		} catch (NoSuchMethodException e) {
			log("Boot failed: class has no main(String[] args): " + className);
			return false;
		} catch (IllegalAccessException e) {
			log("Boot failed: cannot access main method: " + className);
			return false;
		} catch (InvocationTargetException e) {
			log("Application crashed: " + e.getTargetException());
			return false;
		}
	}

	private void initializeScreen() {
		runtime.clear(LOG_BACKGROUND);
		runtime.drawText(8, 8, machine.biosLabel, HEADER_FOREGROUND, LOG_BACKGROUND, false);
		runtime.drawText(8, 20, "Boot sequence start", LOG_FOREGROUND, LOG_BACKGROUND, false);
		present();
		runtime.beep(60, 520);
	}

	private void log(String message) {
		String timestamp = LocalTime.now().format(CLOCK_FORMAT);
		String line = "[" + timestamp + "] " + message;
		System.out.println("[BIOS] " + message);

		logLines.add(line);
		if (logLines.size() > maxLogLines) {
			logLines.remove(0);
		}

		renderLogScreen();
		present();
	}

	private void renderLogScreen() {
		runtime.clear(LOG_BACKGROUND);
		runtime.drawText(8, 8, machine.biosLabel, HEADER_FOREGROUND, LOG_BACKGROUND, false);
		runtime.drawText(8, 20, "Boot log", HEADER_FOREGROUND, LOG_BACKGROUND, false);

		int y = 36;
		int maxCharsPerLine = Math.max(1, runtime.getTextColumns() - 2);
		for (String line : logLines) {
			String clipped = line.length() > maxCharsPerLine ? line.substring(0, maxCharsPerLine) : line;
			runtime.drawText(8, y, clipped, LOG_FOREGROUND, LOG_BACKGROUND, false);
			y += 8;
		}
	}

	private void present() {
		runtime.present();
	}

	private void invokeMainClass(String className, ClassLoader classLoader, String[] applicationArgs)
			throws ClassNotFoundException, NoSuchMethodException, IllegalAccessException, InvocationTargetException {
		Class<?> mainClass = Class.forName(className, true, classLoader);
		Method mainMethod = mainClass.getMethod("main", String[].class);

		int modifiers = mainMethod.getModifiers();
		if (!Modifier.isPublic(modifiers) || !Modifier.isStatic(modifiers)) {
			throw new IllegalAccessException("main method must be public static");
		}

		mainMethod.invoke(null, (Object) (applicationArgs == null ? new String[0] : applicationArgs));
	}

	private String resolveMainClass(Path applicationJarPath) throws IOException {
		try (JarFile jarFile = new JarFile(applicationJarPath.toFile())) {
			if (jarFile.getManifest() != null) {
				Attributes mainAttributes = jarFile.getManifest().getMainAttributes();
				String manifestMainClass = mainAttributes.getValue(Attributes.Name.MAIN_CLASS);
				if (manifestMainClass != null && !manifestMainClass.isBlank()) {
					return manifestMainClass.trim();
				}
			}

			String bootClass = null;
			String mainClass = null;

			Enumeration<JarEntry> entries = jarFile.entries();
			while (entries.hasMoreElements()) {
				JarEntry entry = entries.nextElement();
				String name = entry.getName();
				if (!name.endsWith(".class") || name.contains("$") || name.startsWith("META-INF/")) {
					continue;
				}

				String className = name.substring(0, name.length() - 6).replace('/', '.');
				String simpleName = className.substring(className.lastIndexOf('.') + 1);

				if (simpleName.equalsIgnoreCase("Boot")) {
					bootClass = className;
					break;
				}

				if (mainClass == null && simpleName.equalsIgnoreCase("Main")) {
					mainClass = className;
				}
			}

			if (bootClass != null) {
				return bootClass;
			}

			return mainClass;
		}
	}
}
