package com.github._0x4248.nova;

import com.github._0x4248.nova.Machine.core.Machine;
import com.github._0x4248.nova.Machine.core.MachineProgram;

import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.Arrays;

public class Main {
    private static final String DEFAULT_INTERNAL_APP = "com.github._0x4248.nova_examples.NovaBasic";

    public static void main(String[] args) {
        String targetProgram = args.length > 0 ? args[0] : DEFAULT_INTERNAL_APP;
        String[] programArgs = args.length > 1 ? Arrays.copyOfRange(args, 1, args.length) : new String[0];

        Machine machine = Machine.basic();
        if (!runProgram(targetProgram, machine, programArgs)) {
            System.exit(1);
        }
    }

    private static boolean runProgram(String className, Machine machine, String[] args) {
        try {
            Class<?> programClass = Class.forName(className);

            if (MachineProgram.class.isAssignableFrom(programClass)) {
                MachineProgram program = (MachineProgram) programClass.getDeclaredConstructor().newInstance();
                program.run(machine, args == null ? new String[0] : args);
                return true;
            }

            Method mainMethod = programClass.getMethod("main", String[].class);
            int modifiers = mainMethod.getModifiers();
            if (!Modifier.isPublic(modifiers) || !Modifier.isStatic(modifiers)) {
                throw new IllegalStateException("main(String[]) must be public static");
            }

            mainMethod.invoke(null, (Object) (args == null ? new String[0] : args));
            return true;
        } catch (Exception e) {
            System.err.println("Failed to run program '" + className + "': " + e.getMessage());
            return false;
        }
    }
}