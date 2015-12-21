package net.telesing.scomm.android.core;

import android.content.Context;
import android.content.Intent;
import com.google.gson.Gson;
import com.google.gson.JsonSyntaxException;
import net.telesing.scomm.android.DemService;
import net.telesing.scomm.android.Msg;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by FengJZ on 2015/5/22.
 */
public class DemImplement {

    public static int state() {
        return Demodulator.state();
    }

    public static boolean close(Context context) {
        try {
            Intent intent = new Intent(context, DemService.class);
            context.stopService(intent);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    public static boolean open(Context context) {
        try {
            Intent intent = new Intent(context, DemService.class);
            intent.putExtra(DemService.OP_STR, DemService.OP_OPEN);
            context.startService(intent);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    public static boolean run(Context context) {
        try {
            Intent intent = new Intent(context, DemService.class);
            intent.putExtra(DemService.OP_STR, DemService.OP_RUN);
            context.startService(intent);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    public static List<Msg> parseMsgs(String msgsJSON) {
        List<Msg> msgs = new ArrayList<Msg>();
        try {
            JSONArray msgsJsonObject = new JSONArray(msgsJSON);
            for (int i = 0; i < msgsJsonObject.length(); i++) {
                JSONObject msgJsonObject = msgsJsonObject.getJSONObject(i);
                Msg msg = new Gson().fromJson(msgJsonObject.toString(), Msg.class);
                msgs.add(msg);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        } catch (JsonSyntaxException e) {
            e.printStackTrace();
        }
        return msgs;
    }

}
