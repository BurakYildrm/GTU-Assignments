/**
 * User interface.
 */
public interface UserInterface {
    /**
     * Returns the name of the user.
     * @return the name of the user
     */
    String getName();
    /**
     * Returns the password of the user.
     * @return the password of the user
     */
    String getPassword();
    /**
     * Sets the name of the user.
     * @param name new name
     */
    void setName(String name);
    /**
     * Sets the password of the user.
     * @param password new password
     */
    void setPassword(String password);
}
