/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.gl2jni;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.GestureDetector.OnDoubleTapListener;
import android.view.GestureDetector.OnGestureListener;
import android.view.MotionEvent;
import android.view.WindowManager;
import android.view.GestureDetector;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStreamWriter;
import java.util.Scanner;


public class GL2JNIActivity extends Activity implements OnDoubleTapListener,OnGestureListener
{
    GL2JNIView mView;
    GestureDetector detector;
    @Override protected void onCreate(Bundle icicle) {
    	detector = new GestureDetector(this,(OnGestureListener) this);
    	setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    	Log.e("RUN", "DONE");
    	GL2JNILib.createAssets(getAssets());
    	try {
			writeToSDCard();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	super.onCreate(icicle);
        mView = new GL2JNIView(getApplication());
	setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent e) {
        // MotionEvent reports input details from the touch screen
        // and other input controls. In this case, you are only
        // interested in events where the touch position changed.
    	GL2JNILib.onTouchEvent(e.getAction(), e.getX(), e.getY()); 
    	detector.onTouchEvent(e);
        return true;
    }
    //Write assets data to sdcard
    private boolean writeToSDCard() throws Exception
    {    	
    	String folderName = "GameData";
    	File dir =new File(android.os.Environment.getExternalStorageDirectory(),folderName);
    	String filePath = android.os.Environment.getExternalStorageDirectory().getPath()+File.separator+folderName+File.separator;
    	Log.e("sdpath", filePath);
    	GL2JNILib.setFilePath(filePath);
        if(!dir.exists())
//        	return true;
//        else
        	dir.mkdirs();
        
    	AssetManager myAssetManager = getAssets();
    	String[] filenames = myAssetManager.list("Models");
    	for(int i=0;i<filenames.length;i++)
    	{
    		Log.w("File name", filenames[i]);
    		
			 InputStream ips = myAssetManager.open("Models"+File.separator+filenames[i]);
			Scanner scr = new Scanner(ips);
			StringBuffer strBuf = new StringBuffer();
			
			while(scr.hasNext())
			{
				strBuf.append(scr.nextLine()+"\n");
			}
			
			//Log.w("shaderData",strBuf.toString());
				
				
		   File f = new File(dir+File.separator+filenames[i]+".txt");
		
		   FileOutputStream fOut = new FileOutputStream(f);
		   OutputStreamWriter myOutWriter = new OutputStreamWriter(fOut);
		   myOutWriter.write(strBuf.toString());
		   myOutWriter.close();
		   fOut.close();
    	}
    	 
    	return true;
    }

    @Override
    public boolean onDoubleTap(MotionEvent event) {
        Log.d("Gesture", "onDoubleTap: " + event.toString());
        return true;
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent event) {
        Log.d("Gesture", "onDoubleTapEvent: " + event.toString());
        return true;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent event) {
        Log.d("Gesture", "onSingleTapConfirmed: " + event.toString());
        return true;
    }

	@Override
	public boolean onDown(MotionEvent e) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,
			float velocityY) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void onLongPress(MotionEvent e) {
		// TODO Auto-generated method stub
		Log.d("Gesture", "onLongPress: " + e.toString());
        GL2JNILib.onLongPress();
	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX,
			float distanceY) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void onShowPress(MotionEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public boolean onSingleTapUp(MotionEvent e) {
		// TODO Auto-generated method stub
		return false;
	}
}
