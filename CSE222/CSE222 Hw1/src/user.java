public class user implements userFunctions {
    private String name;
    private String surname;
    private String password;
    private String ID;

    public user(String name, String surname, String ID, String password) {
        this.name = name;
        this.surname = surname;
        this.password = password;
        this.ID = ID;
    }

    /**
     * sets name of the user
     * @param name name which will be set to user
     */
    @Override
    public void setName(String name) {
        this.name = name;
    }

    /**
     * sets ID of the user
     * @param ID ID which will be set to user
     */
    @Override
    public void setID(String ID) {
        this.ID = ID;
    }

    /**
     * sets surname of the user
     * @param surname surname which will be set to user
     */
    @Override
    public void setSurname(String surname) {
        this.surname = surname;
    }

    /**
     * sets password of the user
     * @param password password which will be set to user
     */
    @Override
    public void setPassword(String password) {
        this.password = password;
    }

    /**
     * @return name of the user
     */
    @Override
    public String getName() {
        return name;
    }

    /**
     * @return surname of the user
     */
    @Override
    public String getSurname() {
        return surname;
    }

    /**
     * @return password of the user
     */
    @Override
    public String getPassword() {
        return password;
    }

    /**
     * @return ID of the user
     */
    @Override
    public String getID() {
        return ID;
    }
}
