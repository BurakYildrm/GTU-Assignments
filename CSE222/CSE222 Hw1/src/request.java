public class request {
    enum Permission{
        GRANTED,
        NO_REQUEST,
        PARKED,
        DENIED
    }
    private Permission permission;
    private String employeeID;

    public request(String employeeID) {
        permission = Permission.PARKED;
        this.employeeID = employeeID;
    }

    /**
     * @return permission of request
     */
    public Permission getPermission() {
        return permission;
    }

    /**
     * @return ID of the branch employee
     */
    public String getEmployeeID() {
        return employeeID;
    }

    /**
     * sets the permission of the request
     * @param permission permission which will be set to request
     */
    public void setPermission(Permission permission) {
        this.permission = permission;
    }
}
