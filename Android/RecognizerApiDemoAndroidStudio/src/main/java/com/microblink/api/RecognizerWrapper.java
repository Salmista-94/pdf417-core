package com.microblink.api;

import android.util.Log;

public class RecognizerWrapper {

    static {
        try {
            System.loadLibrary("NativeRecognizer");
        } catch (UnsatisfiedLinkError e) {
            Log.e("RecognizerWrapper", "error loading libNativeRecognizer.so", e);
        }
    }

    public native String init();

    public native void terminate();

    public native String recognize(byte[] bytes);
}
