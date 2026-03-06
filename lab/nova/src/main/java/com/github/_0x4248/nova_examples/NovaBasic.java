package com.github._0x4248.nova_examples;

import com.github._0x4248.nova.Machine.core.Machine;
import com.github._0x4248.nova.Machine.core.MachineProgram;
import com.github._0x4248.nova.Machine.core.Keyboard;
import com.github._0x4248.nova.Machine.core.Video;
import com.github._0x4248.nova.Machine.gpu.GPU;

import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.TreeMap;

public class NovaBasic implements MachineProgram {

    private static final int VIDEO_COLUMNS = 40;
    private static final int VIDEO_ROWS = 25;

    private final TreeMap<Integer, String> program = new TreeMap<>();
    private final Map<String, Integer> variables = new HashMap<>();
    private final List<String> screenLines = new ArrayList<>();
    private GPU gpu;
    private Keyboard keyboard;
    private String currentInput = "";

    public static void main(String[] args) {
        new NovaBasic().run(Machine.basic(), args);
    }

    @Override
    public void run(Machine machine, String[] args) {
        initVideoOut(machine);
        initKeyboardIn(machine);

        outLine("NOVA BASIC V0.1");
        outLine("READY.");
        outLine("Type HELP for commands.");

        if (gpu != null && keyboard != null) {
            runKeyboardLoop();
            if (gpu != null) {
                gpu.shutdown();
            }
            return;
        }

        try (Scanner scanner = new Scanner(System.in)) {
            while (true) {
                System.out.print("> ");
                if (!scanner.hasNextLine()) {
                    break;
                }

                String line = scanner.nextLine().trim();
                appendScreenLine("> " + line);
                if (line.isEmpty()) {
                    continue;
                }

                if (processInputLine(line)) {
                    break;
                }
            }
        }

        if (gpu != null) {
            gpu.shutdown();
        }
    }

    private void runKeyboardLoop() {
        while (true) {
            String line = readLineFromKeyboard();
            if (line == null) {
                continue;
            }

            String trimmed = line.trim();
            if (trimmed.isEmpty()) {
                continue;
            }

            if (processInputLine(trimmed)) {
                break;
            }
        }
    }

    private boolean processInputLine(String line) {
        if (startsWithLineNumber(line)) {
            saveProgramLine(line);
            return false;
        }

        return handleDirectCommand(line);
    }

    private boolean handleDirectCommand(String line) {
        String upper = line.toUpperCase();

        if (upper.equals("RUN")) {
            runProgram();
            return false;
        }
        if (upper.equals("LIST")) {
            listProgram();
            return false;
        }
        if (upper.equals("NEW")) {
            program.clear();
            variables.clear();
            outLine("OK");
            return false;
        }
        if (upper.equals("HELP")) {
            printHelp();
            return false;
        }
        if (upper.equals("VARS")) {
            printVars();
            return false;
        }
        if (upper.equals("EXIT") || upper.equals("QUIT") || upper.equals("BYE")) {
            outLine("BYE");
            return true;
        }

        try {
            executeStatement(line, null, null);
        } catch (BasicRuntimeException e) {
            outLine("? " + e.getMessage());
        }
        return false;
    }

    private boolean startsWithLineNumber(String line) {
        int idx = 0;
        while (idx < line.length() && Character.isDigit(line.charAt(idx))) {
            idx++;
        }
        return idx > 0 && (idx == line.length() || Character.isWhitespace(line.charAt(idx)));
    }

    private void saveProgramLine(String line) {
        int idx = 0;
        while (idx < line.length() && Character.isDigit(line.charAt(idx))) {
            idx++;
        }

        int lineNumber = Integer.parseInt(line.substring(0, idx));
        String statement = line.substring(idx).trim();

        if (statement.isEmpty()) {
            program.remove(lineNumber);
        } else {
            program.put(lineNumber, statement);
        }
    }

    private void listProgram() {
        if (program.isEmpty()) {
            outLine("(empty)");
            return;
        }
        for (Map.Entry<Integer, String> entry : program.entrySet()) {
            outLine(entry.getKey() + " " + entry.getValue());
        }
    }

    private void printHelp() {
        outLine("DIRECT: RUN, LIST, NEW, VARS, HELP, QUIT");
        outLine("PROGRAM: PRINT, LET, GOTO, IF ... THEN <line>, END, REM");
        outLine("EXAMPLE:");
        outLine("10 LET X = 1");
        outLine("20 PRINT X");
        outLine("30 LET X = X + 1");
        outLine("40 IF X < 5 THEN 20");
        outLine("50 END");
    }

    private void printVars() {
        if (variables.isEmpty()) {
            outLine("(no vars)");
            return;
        }

        for (Map.Entry<String, Integer> entry : variables.entrySet()) {
            outLine(entry.getKey() + " = " + entry.getValue());
        }
    }

    private void runProgram() {
        if (program.isEmpty()) {
            outLine("NO PROGRAM");
            return;
        }

        List<Integer> lineNumbers = new ArrayList<>(program.keySet());
        int pc = 0;
        int stepCounter = 0;
        final int maxSteps = 100_000;

        try {
            while (pc >= 0 && pc < lineNumbers.size()) {
                if (++stepCounter > maxSteps) {
                    throw new BasicRuntimeException("TOO MANY STEPS (possible infinite loop)");
                }

                int currentLine = lineNumbers.get(pc);
                String statement = program.get(currentLine);

                ExecutionResult result = executeStatement(statement, lineNumbers, pc);
                if (result.endProgram) {
                    break;
                }
                if (result.nextPc != null) {
                    pc = result.nextPc;
                } else {
                    pc++;
                }
            }

            outLine("READY.");
        } catch (BasicRuntimeException e) {
            outLine("? " + e.getMessage());
        }
    }

    private ExecutionResult executeStatement(String statement, List<Integer> lineNumbers, Integer currentPc) {
        String trimmed = statement.trim();
        if (trimmed.isEmpty()) {
            return ExecutionResult.continueNext();
        }

        String upper = trimmed.toUpperCase();

        if (upper.startsWith("REM") || upper.startsWith("'")) {
            return ExecutionResult.continueNext();
        }

        if (upper.startsWith("PRINT")) {
            String arg = trimmed.length() > 5 ? trimmed.substring(5).trim() : "";
            doPrint(arg);
            return ExecutionResult.continueNext();
        }

        if (upper.startsWith("LET")) {
            String assign = trimmed.substring(3).trim();
            doLet(assign);
            return ExecutionResult.continueNext();
        }

        if (upper.startsWith("IF")) {
            return doIf(trimmed, lineNumbers);
        }

        if (upper.startsWith("GOTO")) {
            String arg = trimmed.substring(4).trim();
            int target = parseLineNumber(arg);
            return ExecutionResult.jump(findLineIndex(lineNumbers, target));
        }

        if (upper.equals("END")) {
            return ExecutionResult.end();
        }

        if (upper.equals("RUN")) {
            runProgram();
            return ExecutionResult.end();
        }

        if (trimmed.matches("^[A-Za-z][A-Za-z0-9_]*\\s*=.*$")) {
            doLet(trimmed);
            return ExecutionResult.continueNext();
        }

        throw new BasicRuntimeException("SYNTAX ERROR: " + statement);
    }

    private void doPrint(String arg) {
        if (arg.isEmpty()) {
            outLine("");
            return;
        }

        if ((arg.startsWith("\"") && arg.endsWith("\"")) || (arg.startsWith("'") && arg.endsWith("'"))) {
            outLine(arg.substring(1, arg.length() - 1));
            return;
        }

        int value = evalExpr(arg);
        outLine(String.valueOf(value));
    }

    private void initVideoOut(Machine machine) {
        try {
            gpu = machine.video().gpu;
            gpu.init(Video.Modes.CGA_TEXT_40x25);
            redrawVideo();
        } catch (Exception ignored) {
            gpu = null;
        }
    }

    private void initKeyboardIn(Machine machine) {
        try {
            keyboard = machine.keyboard();
        } catch (Exception ignored) {
            keyboard = null;
        }
    }

    private void outLine(String text) {
        System.out.println(text);
        appendScreenLine(text);
    }

    private void appendScreenLine(String text) {
        String value = text == null ? "" : text;
        for (String part : wrapLine(value)) {
            screenLines.add(part);
        }

        while (screenLines.size() > VIDEO_ROWS - 2) {
            screenLines.remove(0);
        }

        redrawVideo();
    }

    private List<String> wrapLine(String text) {
        List<String> wrapped = new ArrayList<>();
        if (text.isEmpty()) {
            wrapped.add("");
            return wrapped;
        }

        int start = 0;
        while (start < text.length()) {
            int end = Math.min(start + VIDEO_COLUMNS, text.length());
            wrapped.add(text.substring(start, end));
            start = end;
        }
        return wrapped;
    }

    private void redrawVideo() {
        if (gpu == null) {
            return;
        }

        gpu.clear(0);
        gpu.drawText(0, 0, fitLine("NOVA BASIC"), 15, 0, false);

        int row = 1;
        for (String line : screenLines) {
            if (row >= VIDEO_ROWS - 1) {
                break;
            }
            gpu.drawText(0, row * 8, fitLine(line), 15, 0, false);
            row++;
        }

        String prompt = "> " + currentInput + "_";
        gpu.drawText(0, (VIDEO_ROWS - 1) * 8, fitLine(prompt), 14, 0, false);

        gpu.present();
    }

    private String readLineFromKeyboard() {
        StringBuilder builder = new StringBuilder();
        currentInput = "";
        redrawVideo();

        while (true) {
            Integer keyCode = keyboard.pollKeyCode();
            if (keyCode == null) {
                sleep(10);
                continue;
            }

            if (keyCode == KeyEvent.VK_ENTER) {
                String line = builder.toString();
                appendScreenLine("> " + line);
                System.out.println("> " + line);
                currentInput = "";
                redrawVideo();
                return line;
            }

            if (keyCode == KeyEvent.VK_BACK_SPACE) {
                if (builder.length() > 0) {
                    builder.deleteCharAt(builder.length() - 1);
                    currentInput = builder.toString();
                    redrawVideo();
                }
                continue;
            }

            if (keyCode == KeyEvent.VK_ESCAPE) {
                builder.setLength(0);
                builder.append("QUIT");
                currentInput = builder.toString();
                redrawVideo();
                continue;
            }

            Character mapped = mapKeyCodeToChar(keyCode);
            if (mapped != null) {
                builder.append(mapped);
                currentInput = builder.toString();
                redrawVideo();
            }
        }
    }

    private Character mapKeyCodeToChar(int keyCode) {
        if (keyCode >= KeyEvent.VK_A && keyCode <= KeyEvent.VK_Z) {
            return (char) ('A' + (keyCode - KeyEvent.VK_A));
        }
        if (keyCode >= KeyEvent.VK_0 && keyCode <= KeyEvent.VK_9) {
            return (char) ('0' + (keyCode - KeyEvent.VK_0));
        }
        if (keyCode >= KeyEvent.VK_NUMPAD0 && keyCode <= KeyEvent.VK_NUMPAD9) {
            return (char) ('0' + (keyCode - KeyEvent.VK_NUMPAD0));
        }

        return switch (keyCode) {
            case KeyEvent.VK_SPACE -> ' ';
            case KeyEvent.VK_PERIOD, KeyEvent.VK_DECIMAL -> '.';
            case KeyEvent.VK_COMMA -> ',';
            case KeyEvent.VK_MINUS, KeyEvent.VK_SUBTRACT -> '-';
            case KeyEvent.VK_EQUALS -> '=';
            case KeyEvent.VK_PLUS, KeyEvent.VK_ADD -> '+';
            case KeyEvent.VK_SLASH, KeyEvent.VK_DIVIDE -> '/';
            case KeyEvent.VK_ASTERISK, KeyEvent.VK_MULTIPLY -> '*';
            case KeyEvent.VK_SEMICOLON -> ';';
            case KeyEvent.VK_COLON -> ':';
            case KeyEvent.VK_QUOTE -> '\'';
            case KeyEvent.VK_OPEN_BRACKET -> '[';
            case KeyEvent.VK_CLOSE_BRACKET -> ']';
            case KeyEvent.VK_BACK_SLASH -> '\\';
            default -> null;
        };
    }

    private void sleep(long millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    private String fitLine(String text) {
        String value = text == null ? "" : text;
        if (value.length() > VIDEO_COLUMNS) {
            return value.substring(0, VIDEO_COLUMNS);
        }
        return value + " ".repeat(VIDEO_COLUMNS - value.length());
    }

    private void doLet(String assign) {
        int eq = assign.indexOf('=');
        if (eq <= 0) {
            throw new BasicRuntimeException("LET EXPECTS: NAME = EXPR");
        }

        String name = assign.substring(0, eq).trim().toUpperCase();
        if (!name.matches("[A-Z][A-Z0-9_]*")) {
            throw new BasicRuntimeException("BAD VARIABLE NAME: " + name);
        }

        String expr = assign.substring(eq + 1).trim();
        int value = evalExpr(expr);
        variables.put(name, value);
    }

    private ExecutionResult doIf(String statement, List<Integer> lineNumbers) {
        String body = statement.substring(2).trim();
        int thenPos = body.toUpperCase().indexOf("THEN");
        if (thenPos < 0) {
            throw new BasicRuntimeException("IF EXPECTS THEN");
        }

        String cond = body.substring(0, thenPos).trim();
        String thenTarget = body.substring(thenPos + 4).trim();

        if (evaluateCondition(cond)) {
            int target = parseLineNumber(thenTarget);
            return ExecutionResult.jump(findLineIndex(lineNumbers, target));
        }
        return ExecutionResult.continueNext();
    }

    private boolean evaluateCondition(String cond) {
        String[] ops = {"<=", ">=", "<>", "=", "<", ">"};

        for (String op : ops) {
            int idx = cond.indexOf(op);
            if (idx > 0) {
                int left = evalExpr(cond.substring(0, idx).trim());
                int right = evalExpr(cond.substring(idx + op.length()).trim());

                return switch (op) {
                    case "=" -> left == right;
                    case "<>" -> left != right;
                    case "<" -> left < right;
                    case ">" -> left > right;
                    case "<=" -> left <= right;
                    case ">=" -> left >= right;
                    default -> false;
                };
            }
        }

        throw new BasicRuntimeException("BAD IF CONDITION: " + cond);
    }

    private int parseLineNumber(String text) {
        try {
            return Integer.parseInt(text.trim());
        } catch (NumberFormatException e) {
            throw new BasicRuntimeException("BAD LINE NUMBER: " + text);
        }
    }

    private int findLineIndex(List<Integer> lineNumbers, int line) {
        if (lineNumbers == null) {
            throw new BasicRuntimeException("GOTO/IF THEN needs a running program");
        }

        int idx = lineNumbers.indexOf(line);
        if (idx < 0) {
            throw new BasicRuntimeException("UNDEFINED LINE " + line);
        }
        return idx;
    }

    private int evalExpr(String expr) {
        return new ExpressionParser(expr).parse();
    }

    private static final class ExecutionResult {
        private final Integer nextPc;
        private final boolean endProgram;

        private ExecutionResult(Integer nextPc, boolean endProgram) {
            this.nextPc = nextPc;
            this.endProgram = endProgram;
        }

        private static ExecutionResult continueNext() {
            return new ExecutionResult(null, false);
        }

        private static ExecutionResult jump(int nextPc) {
            return new ExecutionResult(nextPc, false);
        }

        private static ExecutionResult end() {
            return new ExecutionResult(null, true);
        }
    }

    private static final class BasicRuntimeException extends RuntimeException {
        private BasicRuntimeException(String message) {
            super(message);
        }
    }

    private final class ExpressionParser {
        private final String input;
        private int pos;

        private ExpressionParser(String input) {
            this.input = input;
        }

        private int parse() {
            int value = parseExpression();
            skipSpaces();
            if (pos != input.length()) {
                throw new BasicRuntimeException("BAD EXPR: " + input);
            }
            return value;
        }

        private int parseExpression() {
            int value = parseTerm();
            while (true) {
                skipSpaces();
                if (match('+')) {
                    value += parseTerm();
                } else if (match('-')) {
                    value -= parseTerm();
                } else {
                    return value;
                }
            }
        }

        private int parseTerm() {
            int value = parseFactor();
            while (true) {
                skipSpaces();
                if (match('*')) {
                    value *= parseFactor();
                } else if (match('/')) {
                    int divisor = parseFactor();
                    if (divisor == 0) {
                        throw new BasicRuntimeException("DIV BY ZERO");
                    }
                    value /= divisor;
                } else {
                    return value;
                }
            }
        }

        private int parseFactor() {
            skipSpaces();

            if (match('(')) {
                int value = parseExpression();
                skipSpaces();
                if (!match(')')) {
                    throw new BasicRuntimeException("MISSING )");
                }
                return value;
            }

            if (match('-')) {
                return -parseFactor();
            }

            if (pos < input.length() && Character.isDigit(input.charAt(pos))) {
                return parseNumber();
            }

            if (pos < input.length() && Character.isLetter(input.charAt(pos))) {
                String name = parseIdentifier().toUpperCase();
                return variables.getOrDefault(name, 0);
            }

            throw new BasicRuntimeException("BAD EXPR: " + input);
        }

        private int parseNumber() {
            int start = pos;
            while (pos < input.length() && Character.isDigit(input.charAt(pos))) {
                pos++;
            }
            return Integer.parseInt(input.substring(start, pos));
        }

        private String parseIdentifier() {
            int start = pos;
            while (pos < input.length() && (Character.isLetterOrDigit(input.charAt(pos)) || input.charAt(pos) == '_')) {
                pos++;
            }
            return input.substring(start, pos);
        }

        private void skipSpaces() {
            while (pos < input.length() && Character.isWhitespace(input.charAt(pos))) {
                pos++;
            }
        }

        private boolean match(char expected) {
            if (pos < input.length() && input.charAt(pos) == expected) {
                pos++;
                return true;
            }
            return false;
        }
    }
}
