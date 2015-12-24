package net.telesing.tscom.adapter;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import net.telesing.tscom.R;
import net.telesing.tscom.common.utils.CommUtils;
import net.telesing.tsdk.tlib.bean.SNBean;

import java.util.ArrayList;
import java.util.List;


public class MsgAdapter extends BaseAdapter {

    private LayoutInflater minflater;
    private Context context;
    private List<SNBean> infoList;

    public MsgAdapter(Context context, List<SNBean> infoList) {
        this.context = context;
        this.infoList = infoList;
        if (infoList == null)
            this.infoList = new ArrayList<SNBean>();
        minflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    }

    public void clear() {
        infoList.clear();
        notifyDataSetChanged();
    }

    public void addInfo(List<SNBean> infos, int type) {
        if (infos == null || infos.size() <= 0)
            return;
        for (int i = 0; i < infos.size(); i++) {
            SNBean msgSn = infos.get(i);
            if (type == 0) {
                infoList.add(msgSn);
            } else {
                infoList.add(0, msgSn);
            }
        }
        notifyDataSetChanged();
    }



    public void addInfo(SNBean snBean) {
        if(snBean != null){
            infoList.add(0, snBean);
        }
        notifyDataSetChanged();
    }

    public void changeView(SNBean snBean){
        if(infoList != null && snBean != null){
            int index = findSvecIndex(snBean);
            if(infoList != null && snBean != null) {
                infoList.remove(index);
                infoList.add(index, snBean);
            }

            notifyDataSetChanged();
        }
    }

    public int findSvecIndex(SNBean snBean){
        if(infoList != null && snBean != null){
            for (int i=0;i<infoList.size();i++){
                SNBean svec = infoList.get(i);
                if(svec.getRec_time() == snBean.getRec_time()){
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
        SNBean msgb = infoList.get(position);
        holder.snTxt.setText(msgb.getSn());
        holder.endTimeTxt.setText(CommUtils.longShortString(msgb.getRec_time()));
        holder.timeTxt.setText(String.valueOf((msgb.getTimes()/1000.0)));
        holder.resTxt.setText(msgb.getResult());
        if(msgb.isHave()){
            holder.msgResTxt.setText("R");
        }else {
            holder.msgResTxt.setText("N");
        }
        holder.snTxt.setTextColor(context.getResources().getColor(msgb.getColor()));
        holder.endTimeTxt.setTextColor(context.getResources().getColor(msgb.getColor()));
        holder.timeTxt.setTextColor(context.getResources().getColor(msgb.getColor()));
        holder.resTxt.setTextColor(context.getResources().getColor(msgb.getColor()));
        holder.msgResTxt.setTextColor(context.getResources().getColor(msgb.getColor()));
        return convertView;
    }


    public Context getContext() {
        return context;
    }

    public void setContext(Context context) {
        this.context = context;
    }

    public List<SNBean> getInfoList() {
        return infoList;
    }

    public void setInfoList(List<SNBean> infoList) {
        this.infoList = infoList;
    }
}
