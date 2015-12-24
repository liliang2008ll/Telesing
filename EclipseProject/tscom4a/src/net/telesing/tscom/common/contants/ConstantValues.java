package net.telesing.tscom.common.contants;


public class ConstantValues
{


    public final static String DOWNLOAD_DIR = "qyt_svec/";

    public static final int TIMEOUT = 10 * 1000;// 超时
    public static final int SETTING_FRAGMENT_INDEX = 0;
    public static final int MESSAGE_FRAGMENT_INDEX = 1;
    public static final int SENIOR_SETTING_FRAGMENT_INDEX = 2;
    public static final int ABOUT_FRAGMENT_INDEX = 3;

    public static final String GET_SVECRESBEAN_BY_DATE_SQL = "SELECT * FROM svec WHERE dates = ?";
    public static final String GET_SNMSG_BY_DATE_SQL = "SELECT * FROM sn_msg WHERE sn = ?";

    public static final String GET_ALL_SQL = "SELECT * FROM svec order by dates desc ";

    public static final String GET_SN = "SELECT * FROM sn_list WHERE sn = ?";
    public static final String FIND_ALL_SN = "SELECT * FROM sn_list ";
}
