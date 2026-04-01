package client.handle;

import java.math.BigDecimal;
import java.util.Currency;

import client.Constants;

public class SafeUI {

    // Functional interface for validation
    @FunctionalInterface
    public interface Validator {
        void validate(String input) throws IllegalArgumentException;
    }

    /**
     * Prompts the user for input using UI.prompt, validates it, and re-prompts if invalid.
     * @param promptMessage The message to display to the user.
     * @param validator The validation function to use.
     * @return The valid input string.
     */
    public static String prompt(String promptMessage, Validator validator) {
        boolean hasRetried = false;
        while (true) {
            String input = UI.prompt(promptMessage);
            try {
                validator.validate(input);
                if(hasRetried) {
                    UI.resetLine();
                }
                return input;
            } catch (IllegalArgumentException e) {
                hasRetried = true;
                UI.printError(e.getMessage());
                UI.cursorUp(); UI.cursorUp(); UI.resetLine();
            }
        }
    }
    
    /**
     * Validates that the name contains only alphabets (A-Z, a-z) and is not empty.
     * Throws IllegalArgumentException if invalid.
     */
    public static void validateName(String name) throws IllegalArgumentException {
        if (name == null || !name.matches("[A-Za-z ]+")) {
            throw new IllegalArgumentException("Name must contain only alphabets and spaces, and cannot be empty.");
        }
    }

    /**
     * Validates that the password contains no hidden (non-printable) characters.
     * Throws IllegalArgumentException if invalid.
     */
    public static void validatePassword(String password) throws IllegalArgumentException {
        if (password == null || !password.matches("^[\\x20-\\x7E]+$")) {
            throw new IllegalArgumentException("Password contains invalid or hidden characters.");
        }
    }
    
    public static void validatePasswordConfirmation(String newPassword, String confirmPassword) throws IllegalArgumentException {
        if (!newPassword.equals(confirmPassword)) {
            throw new IllegalArgumentException("New password and confirmation do not match.");
        }
    }

    /**
     * Validates that the account number contains digits only and is not empty.
     * Throws IllegalArgumentException if invalid.
     */
    public static void validateAccountNumber(String accountNumber) throws IllegalArgumentException {
        if (accountNumber == null || !accountNumber.matches("\\d+")) {
            throw new IllegalArgumentException("Account number must contain digits only and cannot be empty.");
        }
    }

    public static long validateMoneyInput(String input) throws IllegalArgumentException {
        try {
            BigDecimal amount = new BigDecimal(input);
            if (amount.scale() > 2) {
                throw new IllegalArgumentException("Amount cannot have more than 2 decimal places.");
            }
            if (amount.compareTo(BigDecimal.ZERO) < 0) {
                throw new IllegalArgumentException("Amount cannot be negative.");
            }
            return parseMoneyNumber(input);
        } catch (NumberFormatException e) {
            throw new IllegalArgumentException("Invalid Money Format");
        }
    }

    public static void validateYesNo(String input) throws IllegalArgumentException {
        if (input == null || 
            !(input.equalsIgnoreCase("y") || input.equalsIgnoreCase("n"))) {
            throw new IllegalArgumentException("Please enter 'y' or 'n' only.");
        }
    }
    public static void validateDepositWithdraw(String input) throws IllegalArgumentException {
        if (input == null || 
            !(input.equalsIgnoreCase("d") || input.equalsIgnoreCase("w"))) {
            throw new IllegalArgumentException("Please enter 'd' (deposit) or 'w' (withdraw) only.");
        }
    }

    public static long parseMoneyNumber(String input) {
        // BigDecimal handles the "clean" multiplication by 100
        return new BigDecimal(input)
                .multiply(new BigDecimal("100"))
                .longValue();
    }

    public static String moneyToString(long amountInCents) {
        BigDecimal amount = new BigDecimal(amountInCents).divide(new BigDecimal("100"));
        return amount.setScale(2, java.math.RoundingMode.HALF_UP).toString();
    }
    
    
    public static void validateCurrency(String input) throws IllegalArgumentException {
        if (input.length() != 3 || !Character.isLetter(input.charAt(0))) {
            throw new IllegalArgumentException("Currency must be a three-letter code.");
        }
        if (!input.equalsIgnoreCase("SGD") && !input.equalsIgnoreCase("USD") && !input.equalsIgnoreCase("EUR")) {
            throw new IllegalArgumentException("Unsupported currency. Please enter SGD, USD, or EUR.");
        }
    }
    
    
    public static char currencyCodeToChar(String currency) {
        switch (currency.toUpperCase()) {
            case "SGD":
                return Constants.SGD;
            case "USD":
                return Constants.USD;
            case "EUR":
                return Constants.EUR;
            default:
                throw new IllegalArgumentException("Unsupported currency. Please enter SGD, USD, or EUR.");
        }
    }

    public static String currencyCodeToString(char currency) {
        switch (currency) {
            case Constants.SGD:
                return "SGD";
            case Constants.USD:
                return "USD";
            case Constants.EUR:
                return "EUR";
            default:
                return "Unknown Currency";
        }
    }


}
