package com.microblink.api;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import test.photopay.api.R;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

public class MenuActivity extends Activity {

    public static final int CAMERA_REQUEST_CODE = 0x101;
    // file that will hold the image taken from camera
    private String mFile = Environment.getExternalStorageDirectory().getPath() + "/my-photo.jpg";
    // image bytes (required for recognition repeatability)
    private byte[] mImgData = null;
    // tag for logcat
    public static final String TAG = "PhotoPayDemo";
    private Button mScanButton;
    private Button mTakePhotoButton;
    private ImageView mImgView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        mScanButton = (Button) findViewById(R.id.btnScan);
        mTakePhotoButton = (Button) findViewById(R.id.btnTakePhoto);
        mImgView = (ImageView) findViewById(R.id.imgImage);

        // if this is a PDF URL, automatically start upload
        Intent intent = getIntent();
        if (Intent.ACTION_VIEW.equals(intent.getAction())) {
            // URL has been intercepted from other activity
            Uri data = intent.getData();
            if ("file".equals(data.getScheme())) {
                String fileURL = intent.getData().toString();
                fileURL = fileURL.replace("file://", "").replace("%20", " ");
                try {
                    readStream(new FileInputStream(new File(fileURL)));
                } catch (FileNotFoundException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                } catch (IOException exc) {
                    // TODO Auto-generated catch block
                    exc.printStackTrace();
                }
            } else if ("content".equals(data.getScheme())) {
                try {
                    readStream(this.getContentResolver().openInputStream(data));
                } catch (FileNotFoundException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                    finish();
                } catch (IOException exc) {
                    // TODO Auto-generated catch block
                    exc.printStackTrace();
                }
            }
        } else {
            try {
                readStream(getAssets().open("dual-barcode-sample.png"));
            } catch (IOException exc) {
                // TODO Auto-generated catch block
                exc.printStackTrace();
            }
        }
        showBitmap();
    }

    private void readStream(InputStream is) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        copyStream(is, baos, 1024);
        mImgData = baos.toByteArray();
    }

    private void showBitmap() {
        // load bitmap from stream
        Bitmap bitmap = BitmapFactory.decodeByteArray(mImgData, 0, mImgData.length);
        mImgView.setImageBitmap(bitmap);
    }

    private void copyStream(InputStream is, OutputStream os, int bufferSize) throws IOException {
        int count;
        final byte[] buffer = new byte[bufferSize];
        while ((count = is.read(buffer)) > 0) {
            os.write(buffer, 0, count);
        }
        is.close();
    }

    private void startCamera() {
        Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(new File(mFile)));
        startActivityForResult(takePictureIntent, CAMERA_REQUEST_CODE);
    }

    public void takePhotoHandler(View view) {
        startCamera();
    }

    public void scanButtonHandler(View view) {
        new RecognizeTask().execute();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode == RESULT_OK) {
            try {
                readStream(new FileInputStream(new File(mFile)));
                new File(mFile).delete();
                showBitmap();
            } catch (Exception e) {
                e.printStackTrace();
                Toast.makeText(this, e.getMessage(), Toast.LENGTH_LONG).show();
            }
        } else {
            Toast.makeText(this, "Cancelled", Toast.LENGTH_LONG).show();
        }
    }

    private class RecognizeTask extends AsyncTask<Void, Void, String> {
        // recognizer that performs native recognition
        private RecognizerWrapper mRecognizer = new RecognizerWrapper();
        private ProgressDialog mProgress = null;

        @Override
        protected void onPreExecute() {
            mTakePhotoButton.setEnabled(false);
            mScanButton.setEnabled(false);
            // show progress dialog
            mProgress = ProgressDialog.show(MenuActivity.this, "Processing", "Processing");
        }

        @Override
        protected String doInBackground(Void... params) {
            String errorStr = mRecognizer.init();
            if(errorStr!=null && !"".equals(errorStr)) {
                mRecognizer.terminate();
                return errorStr;
            }
            String s = mRecognizer.recognize(mImgData);
            mRecognizer.terminate();
            return s;
        }

        @Override
        protected void onPostExecute(String result) {
            mTakePhotoButton.setEnabled(true);
            mScanButton.setEnabled(true);
            mProgress.dismiss();
//            Toast.makeText(MenuActivity.this, result, Toast.LENGTH_LONG).show();
            // show dialog with result
            AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(MenuActivity.this);
            alertDialogBuilder.setTitle("result").setMessage(result).setNeutralButton("OK", new DialogInterface.OnClickListener() {

                @Override
                public void onClick(DialogInterface dialog, int which) {
                    if (dialog != null) {
                        dialog.dismiss();
                    }
                }
            }).setCancelable(false).create().show();;
        }
    }
}
