/**
 * Class that represents a user.
 */
public abstract class User implements UserInterface{
    /**
     * name of the user
     */
    protected String name;
    /**
     * password of the user
     */
    protected String password;

    /**
     * Constructs a user with given name and password.
     * @param name name of the user
     * @param password password of the user
     */
    public User(String name,String password) {
        this.name = name;
        this.password = password;
    }

    /**
     * Returns the name of the user.
     * @return the name of the user
     */
    public String getName() {
        return name;
    }

    /**
     * Returns the password of the user.
     * @return the password of the user
     */
    public String getPassword() {
        return password;
    }

    /**
     * Sets the name of the user.
     * @param name new name
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
     * Sets the password of the user.
     * @param password new password
     */
    public void setPassword(String password) {
        this.password = password;
    }
}
