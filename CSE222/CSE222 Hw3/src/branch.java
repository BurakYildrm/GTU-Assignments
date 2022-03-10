/**
 * class that represent one branch
 */
public class branch extends storeHouse implements requiredFields {
    private String name;
    private int[][][] branchWishList;

    /**
     * constructs a new branch with given parameters
     * @param name name of the branch
     * @param officeChair stock of office chairs
     * @param officeDesk stock of office desks
     * @param meetingTable stock of meeting tables
     * @param bookCase stock of bookcases
     * @param officeCabinet stock of office cabinets
     */
    public branch(String name, int[][] officeChair, int[][] officeDesk, int[][] meetingTable,
                  int[][] bookCase, int[][] officeCabinet) {
        super(officeChair, officeDesk, meetingTable, bookCase, officeCabinet);
        this.name = name;
        branchWishList = new int[modelNums.length][0][0];
        for(int i = 0; i < modelNums.length; i++){
            branchWishList[i] = new int[modelNums[i]][colorNums[i]];
        }
    }

    /**
     * @return wishlist of the branch
     */
    public int[][][] getBranchWishList() {
        return branchWishList;
    }

    /**
     * @return name of the branch
     */
    public String getName() {
        return name;
    }
}
