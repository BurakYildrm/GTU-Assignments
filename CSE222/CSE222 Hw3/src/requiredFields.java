/**
 * holds the default values for the initialization of the system
 */
public interface requiredFields {
    int EMPLOYEE_PER_BRANCH = 5;
    int DEFAULT_BRANCH_NUM = 4;
    int DEFAULT_ADMIN_NUM = 1;
    int MAX_EMPLOYEE_NUM = 10;
    int DEFAULT_STOCK_NUM = 20;
    int[] modelNums = {furniture.OFFICE_CHAIR_MODEL_NUM, furniture.OFFICE_DESK_MODEL_NUM, furniture.MEETING_TABLE_MODEL_NUM,
            furniture.BOOK_CASE_MODEL_NUM, furniture.OFFICE_CABINET_MODEL_NUM};
    int[] colorNums = {furniture.OFFICE_CHAIR_COLOR_NUM, furniture.OFFICE_DESK_COLOR_NUM, furniture.MEETING_TABLE_COLOR_NUM,
            furniture.BOOK_CASE_COLOR_NUM, furniture.OFFICE_CABINET_COLOR_NUM};
    String[] productNames = {"office chair", "office desk", "meeting table", "bookcase", "office cabinet"};
}