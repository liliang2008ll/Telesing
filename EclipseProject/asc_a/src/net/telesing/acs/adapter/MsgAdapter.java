package net.telesing.acs.adapter;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import net.telesing.acs.R;
import net.telesing.acs.common.utils.CommUtils;
import net.telesing.scomm.android.SvecResBean;

import java.util.ArrayList;
import java.util.List;


public class MsgAdapter extends BaseAdapter {

    private LayoutInflater minflater;
    private Context context;
    private List<SvecResBean> infoList;

    public MsgAdapter(Context context, List<SvecResBean> infoList) {
        this.context = context;
        this.infoList = infoList;
        if (infoList == null)
            this.infoList = new ArrayList<SvecResBean>();
        minflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    }

    public void clear() {
        infoList.clear();
        notifyDataSetChanged();
    }

    public void addInfo(List<SvecResBean> infos, int type) {
        if (infos == null || infos.size() <= 0)
            return;
        for (int i = 0; i < infos.size(); i++) {
            SvecResBean msgSn = infos.get(i);
            if (type == 0) {
                infoList.add(msgSn);
            } else {
                infoList.add(0, msgSn);
            }
        }
        notifyDataSetChanged();
    }



    public void addInfo(SvecResBean svecResBean) {
        if(svecResBean != null){
            infoList.add(0, svecResBean);
        }
        notifyDataSetChanged();
    }

    public void changeView(SvecResBean svecResBean){
        if(infoList != null && svecResBean != null){
            int index = findSvecIndex(svecResBean);
            if(infoList != null && svecResBean != null) {
                infoList.remove(index);
                infoList.add(index, svecResBean);
            }

            notifyDataSetChanged();
        }
    }

    public int findSvecIndex(SvecResBean svecResBean){
        if(infoList != null && svecResBean != null){
            for (int i=0;i<infoList.size();i++){
                SvecResBean svec = infoList.get(i);
                if(svec.getEndTime() == svecResBean.getEndTime()){
                    return i;
                }
            }
        }
        return 0;
    }

    public class ViewHolder {
        public TextView snTxt, endTimeTxt,timeTxt,resTxt, msgResTxt;
    }

    @Override
    public int getCount() {
        return infoList.size();
    }

    @Override
    public Object getItem(int position) {
        return infoList.get(position);
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder holder = null;
        if (convertView == null) {
            holder = new ViewHolder();
            convertView = this.minflater.inflate(R.layout.message_item, null);
            holder.snTxt = (TextView) convertView.findViewById(R.id.sn_txt);
            holder.endTimeTxt = (TextView) convertView.findViewById(R.id.end_time_txt);
            holder.timeTxt = (TextView) convertView.findViewById(R.id.time_txt);
            holder.resTxt = (TextView) convertView.findViewById(R.id.res_txt);
            holder.msgResTxt = (TextView) convertView.findViewById(R.id.msg_res_txt);
            convertView.setTag(holder);
        } else {
            holder = (ViewHolder) convertView.getTag();
        }
        SvecResBean msgb = infoList.get(position);
        holder.snTxt.setText(String.valueOf(msgb.getBeaconSn()));
        holder.endTimeTxt.setText(CommUtils.longShortString(msgb.getEndTime()));
        String totalTime = CommUtils.getDiffSeconds(msgb.getEndTime(), msgb.getStartTime());
        holder.timeTxt.setText(totalTime.length() == 4?totalTime+"0":totalTime);
        holder.resTxt.setText(msgb.getResult());
        holder.msgResTxt.setText(msgb.getMsgRes());
        holder.snTxt.setTextColor(context.getResources().getColor(msgb.getBg_res()));
        holder.endTimeTxt.setTextColor(context.getResources().getColor(msgb.getBg_res()));
        holder.timeTxt.setTextColor(context.getResources().getColor(msgb.getBg_res()));
        holder.resTxt.setTextColor(context.getResources().getColor(msgb.getBg_res()));
        holder.msgResTxt.setTextColor(context.getResources().getColor(msgb.getBg_res()));
        return convertView;
    }


    public Context getContext() {
        return context;
    }

    public void setContext(Context context) {
        this.context = context;
    }

    public List<SvecResBean> getInfoList() {
        return infoList;
    }

    public void setInfoList(List<SvecResBean> infoList) {
        this.infoList = infoList;
    }
}
