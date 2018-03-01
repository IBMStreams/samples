package com.ibm.streamsx.bridge;

import junit.framework.TestCase;
import org.junit.BeforeClass;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;
import java.util.Arrays;
import java.util.List;

public class LibertyProxyTest extends TestCase {
    String AppUrl = null;
    @BeforeClass
    public static void setUpBeforeClass() throws Exception {
        String AppUrl = System.getProperty("appUrl");
    }
    public LibertyProxyTest(String name) {
        super(name);
        AppUrl = System.getProperty("appUrl", "proxystream.mybluemix.net");
        System.out.println(String.format("Using '%s' as root of application URL (appUrl).", AppUrl));
    }
    public void testStreamsToUpperApplication() {
        List<String> strs = Arrays.asList("toUpper", "toUpper+again", "toUpper+UPPER+lower+UPPER");
        for (String str : strs) {
            try {
                String urlStr = "https://"+ AppUrl + "/myStreams/rest/ports/analyze/0?" + str;
                URL url = new URL(urlStr);
                HttpURLConnection con = (HttpURLConnection) url.openConnection();
                con.setRequestMethod("GET");
                assertEquals(200, con.getResponseCode());
                assertEquals("OK", con.getResponseMessage());
                BufferedReader in = new BufferedReader(new InputStreamReader(con.getInputStream()));
                String respBuf = in.readLine();
                assertEquals(String.format("request:%s expected response of '%s' received '%s'.", urlStr, str.toUpperCase(), respBuf), str.toUpperCase(), respBuf);
            } catch (ProtocolException e) {
                e.printStackTrace();
            } catch (MalformedURLException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void testIsTopologyActive() {

        int exceed = 4000;
        try {
            String urlStr = "https://"+ AppUrl + "/TopoDisplay";
            URL url = new URL(urlStr);
            HttpURLConnection con = (HttpURLConnection) url.openConnection();
            con.setRequestMethod("GET");
            assertEquals(200, con.getResponseCode());
            assertEquals("OK", con.getResponseMessage());
            BufferedReader in = new BufferedReader(new InputStreamReader(con.getInputStream()));
            int len;
            int tot = 0;
            String inStr = null;
            while (null != (inStr = in.readLine())) {
                tot += inStr.length();
            }
            assertTrue(String.format("Expected '%s' to exceed %d was %d",urlStr, exceed, tot), tot > exceed );
        } catch (ProtocolException e) {
            e.printStackTrace();
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    public void testStreamsResource() {
        int exceed = 1000;
        try {
            String urlStr = "https://"+ AppUrl + "/streamsx.inet.resources";
            URL url = new URL(urlStr);
            HttpURLConnection con = (HttpURLConnection) url.openConnection();
            con.setRequestMethod("GET");
            assertEquals(200, con.getResponseCode());
            assertEquals("OK", con.getResponseMessage());
            BufferedReader in = new BufferedReader(new InputStreamReader(con.getInputStream()));
            int tot = 0;
            String inStr = null;
            while (null != (inStr = in.readLine())) {
                tot += inStr.length();
            }
            assertTrue(String.format("Expected '%s' to exceed %d was %d",urlStr, exceed, tot), tot > exceed );
        } catch (ProtocolException e) {
            e.printStackTrace();
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

}
