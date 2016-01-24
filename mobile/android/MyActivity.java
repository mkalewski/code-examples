package com.example.helloworld;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

import android.app.Activity;
import android.os.Bundle;
import android.os.StrictMode;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MyActivity extends Activity {

EditText textOut;
TextView textIn;

 @Override
 public void onCreate(Bundle savedInstanceState) {
     super.onCreate(savedInstanceState);

     StrictMode.ThreadPolicy policy =
         new StrictMode.ThreadPolicy.Builder().permitAll().build();
     StrictMode.setThreadPolicy(policy);

     setContentView(R.layout.main);

     textOut = (EditText)findViewById(R.id.textout);
     Button buttonSend = (Button)findViewById(R.id.send);
     textIn = (TextView)findViewById(R.id.textin);
     buttonSend.setOnClickListener(buttonSendOnClickListener);
 }

 Button.OnClickListener buttonSendOnClickListener
 = new Button.OnClickListener(){

@Override
public void onClick(View arg0) {
 Socket socket = null;
 DataOutputStream dataOutputStream = null;
 DataInputStream dataInputStream = null;
 byte[] buffor = new byte[1024];

 try {
  socket = new Socket("192.168.1.1", 1234);
  dataOutputStream = new DataOutputStream(socket.getOutputStream());
  dataInputStream = new DataInputStream(socket.getInputStream());
  dataOutputStream.writeUTF(textOut.getText().toString());
  dataInputStream.read(buffor);
  textIn.setText(new String(buffor, "UTF-8"));
 } catch (UnknownHostException e) {
  e.printStackTrace();
 } catch (IOException e) {
  e.printStackTrace();
 }
 finally{
  if (socket != null){
   try {
    socket.close();
   } catch (IOException e) {
    e.printStackTrace();
   }
  }

  if (dataOutputStream != null){
   try {
    dataOutputStream.close();
   } catch (IOException e) {
    e.printStackTrace();
   }
  }

  if (dataInputStream != null){
   try {
    dataInputStream.close();
   } catch (IOException e) {
    e.printStackTrace();
   }
  }
 }
}};
}
