package com.example.wiffi1;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;
import java.util.List;

import android.content.Context;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.test.AndroidTestCase;
import android.util.Log;
//import cn.itcast.domain.Video;
//import cn.itcast.news.MainActivity;
//import cn.itcast.service.VideoService;

public class MacAddr extends AndroidTestCase {
    private static final String TAG = "MacAddr";
    
    public void testLocalIpAndMac(){
        Log.i(TAG, "IP: "+getLocalIpAddress()+", MAC: "+getLocalMacAddress());
    }

    /**
     * 获取Android本机IP地址
     * 
     * @return
     */
    public String getLocalIpAddress() {
        try {
            for (Enumeration<NetworkInterface> en = NetworkInterface
                    .getNetworkInterfaces(); en.hasMoreElements();) {
                NetworkInterface intf = en.nextElement();
                for (Enumeration<InetAddress> enumIpAddr = intf
                        .getInetAddresses(); enumIpAddr.hasMoreElements();) {
                    InetAddress inetAddress = enumIpAddr.nextElement();
                    if (!inetAddress.isLoopbackAddress()) {
                        return inetAddress.getHostAddress().toString();
                    }
                }
            }
        } catch (SocketException ex) {
            Log.e("WifiPreference IpAddress", ex.toString());
        }
        return null;
    }

    /**
     * 获取Android本机MAC
     * 
     * @return
     */
    private String getLocalMacAddress() {
        WifiManager wifi = (WifiManager) this.getContext().getSystemService(Context.WIFI_SERVICE);
        WifiInfo info = wifi.getConnectionInfo();
        return info.getMacAddress();
    } 
}
