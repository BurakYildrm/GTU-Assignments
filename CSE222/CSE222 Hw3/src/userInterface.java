public interface userInterface {
    /**
     * sets name of the user
     * @param name name which will be set to user
     */
    void setName(String name);
    /**
     * sets ID of the user
     * @param ID ID which will be set to user
     */
    void setID(String ID);
    /**
     * sets surname of the user
     * @param surname surname which will be set to user
     */
    void setSurname(String surname);
    /**
     * sets password of the user
     * @param password password which will be set to user
     */
    void setPassword(String password);
    /**
     * @return name of the user
     */
    String getName();
    /**
     * @return surname of the user
     */
    String getSurname();
    /**
     * @return password of the user
     */
    String getPassword();
    /**
     * @return ID of the user
     */
    String getID();
}
