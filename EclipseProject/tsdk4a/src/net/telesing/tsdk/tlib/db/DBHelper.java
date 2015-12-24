package net.telesing.tsdk.tlib.db;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabase.CursorFactory;
import android.database.sqlite.SQLiteOpenHelper;


public class DBHelper extends SQLiteOpenHelper {

    public static String DB_NAME = "svec_qy.db";
    public static int DB_VERSION =2;

    public DBHelper(Context context, String name, CursorFactory factory, int version) {
        super(context, DB_NAME, null, DB_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL("CREATE TABLE IF NOT EXISTS sca_section"
                + "(id INTEGER PRIMARY KEY AUTOINCREMENT,packageName VARCHAR(200),section VARCHAR(50),validate INTEGER,sid INTEGER,state INTEGER,upt_date INTEGER,org_name VARCHAR(200) )");
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("CREATE TABLE IF NOT EXISTS sca_section"
                + "(id INTEGER PRIMARY KEY AUTOINCREMENT,packageName VARCHAR(200),section VARCHAR(50),validate INTEGER,sid INTEGER,state INTEGER,upt_date INTEGER,org_name VARCHAR(200) )");
    }


    public Cursor execQuery(String query, SQLiteDatabase sdb) {
        return sdb.rawQuery(query, null);
    }

    /**
     * 数据添加
     *
     * @param table
     * @param nullColumnHack
     * @param values
     * @return
     * @throws SQLException
     */
    public long insertSQL(String table, String nullColumnHack, ContentValues values, SQLiteDatabase sdb) throws SQLException {
        return sdb.insert(table, nullColumnHack, values);
    }

    /**
     * 数据跟新
     *
     * @param table
     * @param values
     * @param whereClause
     * @param whereArgs
     * @return
     * @throws SQLException
     */
    public int updateSQL(String table, ContentValues values, String whereClause, String[] whereArgs, SQLiteDatabase sdb) throws SQLException {
        return sdb.update(table, values, whereClause, whereArgs);
    }

    /**
     * 删除数据
     *
     * @param table
     * @param whereClause
     * @param whereArgs
     * @return
     * @throws SQLException
     */
    public int deleteSQL(String table, String whereClause, String[] whereArgs, SQLiteDatabase sdb) throws SQLException {
        return sdb.delete(table, whereClause, whereArgs);
    }

    /**
     * 有参数查询
     *
     * @param query
     * @param param
     * @return
     * @throws SQLException
     */
    public Cursor execQueryParam(String query, String[] param, SQLiteDatabase sdb) throws SQLException {
        return sdb.rawQuery(query, param);

    }

}
