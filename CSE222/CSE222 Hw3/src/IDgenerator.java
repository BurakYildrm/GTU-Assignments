public abstract class IDgenerator {
    /**
     * @return generated ID
     */
    public abstract String generateID(automation_system system);
}

/**
 * generates IDs for employees
 */
class branchEmployeeIDgenerator extends IDgenerator {
    @Override
    public String generateID(automation_system system){
        int num = Integer.parseInt(system.getBranchEmployees().get(system.getBranchEmployees().size() - 1).getID());
        return String.valueOf(num + 1);
    }
}

/**
 * generates IDs for customers
 */
class customerIDgenerator extends IDgenerator {
    @Override
    public String generateID(automation_system system){
        String id = "67";
        for(int i = 2 + String.valueOf(system.getCustomers().size() + 1).length(); i < 5; i++){
            id = new String(id + "0");
        }
        id = new String(id + (system.getCustomers().size() + 1));
        return id;
    }
}