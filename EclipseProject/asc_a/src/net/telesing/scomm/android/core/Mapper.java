package net.telesing.scomm.android.core;

import com.google.gson.Gson;
import net.telesing.scomm.android.Cookie;
import org.apache.http.HttpResponse;
import org.apache.http.HttpVersion;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.conn.ClientConnectionManager;
import org.apache.http.conn.scheme.PlainSocketFactory;
import org.apache.http.conn.scheme.Scheme;
import org.apache.http.conn.scheme.SchemeRegistry;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.impl.conn.tsccm.ThreadSafeClientConnManager;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.CoreConnectionPNames;
import org.apache.http.params.HttpProtocolParams;
import org.apache.http.protocol.HTTP;
import org.apache.http.util.EntityUtils;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by FengJZ on 2015/5/26.
 */
public class Mapper {

    public static String mapping(List<String> snList,Cookie cookie) {
        try {
            List<NameValuePair> params = new ArrayList<NameValuePair>();
            params.add(new BasicNameValuePair("sn", new Gson().toJson(snList)));
            // 新建HttpPost对象
            HttpPost httpPost = new HttpPost(cookie.getUrlString(Cookie.QUERY_MSG_BY_SN_URL));
            // 设置参数实体
            httpPost.setEntity(new UrlEncodedFormEntity(params, HTTP.UTF_8));
            // 获取HttpClient对象
            HttpClient httpClient = getClient();
            // 连接超时
            httpClient.getParams().setParameter(CoreConnectionPNames.CONNECTION_TIMEOUT, 30000);
            // 请求超时
            httpClient.getParams().setParameter(CoreConnectionPNames.SO_TIMEOUT, 30000);
            // 获取HttpResponse实例
            HttpResponse httpResp = httpClient.execute(httpPost);
            // 判断是否请求成功
            if (httpResp.getStatusLine().getStatusCode() == 200) {
                String jsonString = EntityUtils.toString(httpResp.getEntity(), "UTF-8");
                if (jsonString != null) {
                    JSONObject jsonObject = new JSONObject(jsonString);
                    JSONObject resJsonObject = jsonObject.getJSONObject("res");
                    String code = resJsonObject.getString("code");
                    if ("0000".equals(code)) {
                        return jsonObject.getString("datas");
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    private static HttpClient getClient() {
        BasicHttpParams params = new BasicHttpParams();
        HttpProtocolParams.setVersion(params, HttpVersion.HTTP_1_1);
        HttpProtocolParams.setContentCharset(params, HTTP.DEFAULT_CONTENT_CHARSET);
        HttpProtocolParams.setUseExpectContinue(params, true);
        SchemeRegistry schReg = new SchemeRegistry();
        schReg.register(new Scheme("http", PlainSocketFactory.getSocketFactory(), 80));
        ClientConnectionManager connMgr = new ThreadSafeClientConnManager(params, schReg);
        return new DefaultHttpClient(connMgr, params);
    }

}
