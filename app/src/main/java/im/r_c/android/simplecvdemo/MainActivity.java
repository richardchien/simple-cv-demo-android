package im.r_c.android.simplecvdemo;

import android.content.Intent;
import android.databinding.DataBindingUtil;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Toast;

import com.theartofdev.edmodo.cropper.CropImage;

import java.io.FileNotFoundException;

import im.r_c.android.simplecvdemo.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity implements View.OnClickListener, AdapterView.OnItemSelectedListener {
    private static final String TAG = "MainActivity";

    static {
        System.loadLibrary("native-lib");
    }

    private ActivityMainBinding mBinding;
    private Bitmap mSourceBitmap;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mBinding = DataBindingUtil.setContentView(this, R.layout.activity_main);

        mBinding.btnChoose.setOnClickListener(this);
        mBinding.ivImage.setOnTouchListener(new ImageViewZoomTouchListener());

        ArrayAdapter<String> arrayAdapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, effects);
        arrayAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mBinding.spnEffect.setAdapter(arrayAdapter);
        mBinding.spnEffect.setSelection(0);
        mBinding.spnEffect.setOnItemSelectedListener(this);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch (requestCode) {
            case CropImage.PICK_IMAGE_CHOOSER_REQUEST_CODE: {
                if (resultCode == RESULT_OK) {
                    CropImage.activity(data.getData())
                            .setActivityTitle("裁剪")
                            .setAspectRatio(1, 1)
                            .start(this);
                }
                break;
            }
            case CropImage.CROP_IMAGE_ACTIVITY_REQUEST_CODE: {
                if (resultCode == RESULT_OK) {
                    CropImage.ActivityResult result = CropImage.getActivityResult(data);
                    try {
                        mSourceBitmap = BitmapFactory.decodeStream(getContentResolver().openInputStream(result.getUri()));
                        mBinding.ivImage.setImageBitmap(mSourceBitmap);
                    } catch (FileNotFoundException e) {
                        Toast.makeText(this, "读取图片失败", Toast.LENGTH_SHORT).show();
                        e.printStackTrace();
                    }
                }
                break;
            }
        }
    }

    private int[] getColorArray(Bitmap bitmap) {
        final int w = bitmap.getWidth();
        final int h = bitmap.getHeight();
        int[] result = new int[w * h];
        bitmap.getPixels(result, 0, w, 0, 0, w, h);
        return result;
    }

    private void convertColorArrayToBitmap(int[] colorArray, Bitmap bitmap) {
        final int w = bitmap.getWidth();
        final int h = bitmap.getHeight();
        bitmap.setPixels(colorArray, 0, w, 0, 0, w, h);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btn_choose: {
                mBinding.spnEffect.setSelection(0);
                Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
                intent.setType("image/*");
                startActivityForResult(intent, CropImage.PICK_IMAGE_CHOOSER_REQUEST_CODE);
                break;
            }
        }
    }

    private String[] effects = {"原图", "灰度化 - R 分量", "灰度化 - G 分量", "灰度化 - B 分量",
            "灰度化 - 最大值", "灰度化 - 平均值", "灰度化 - 加权",
            "二值化", "反色", "灰度均衡化", "高斯平滑", "Canny 边缘检测"};

    private native void nativeBitmapGrayingR(int[] colorArray, int width, int height);

    private native void nativeBitmapGrayingG(int[] colorArray, int width, int height);

    private native void nativeBitmapGrayingB(int[] colorArray, int width, int height);

    private native void nativeBitmapGrayingMax(int[] colorArray, int width, int height);

    private native void nativeBitmapGrayingAVG(int[] colorArray, int width, int height);

    private native void nativeBitmapGrayingWAVG(int[] colorArray, int width, int height);

    private native void nativeBitmapBinary(int[] colorArray, int width, int height);

    private native void nativeBitmapInverse(int[] colorArray, int width, int height);

    private native void nativeBitmapEqualizationHist(int[] colorArray, int width, int height);

    private native void nativeBitmapGaussianSmooth(int[] colorArray, int width, int height);

    private native void nativeBitmapCanny(int[] colorArray, int width, int height);

    @Override
    public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
        if (mSourceBitmap == null) {
            mBinding.spnEffect.setSelection(0);
            return;
        }

        Bitmap modifiedBitmap = mSourceBitmap.copy(mSourceBitmap.getConfig(), true);
        boolean modified = true;

        int[] colorArray = getColorArray(modifiedBitmap);
        switch (i) {
            case 1:
                nativeBitmapGrayingR(colorArray, modifiedBitmap.getWidth(), modifiedBitmap.getHeight());
                break;
            case 2:
                nativeBitmapGrayingG(colorArray, modifiedBitmap.getWidth(), modifiedBitmap.getHeight());
                break;
            case 3:
                nativeBitmapGrayingB(colorArray, modifiedBitmap.getWidth(), modifiedBitmap.getHeight());
                break;
            case 4:
                nativeBitmapGrayingMax(colorArray, modifiedBitmap.getWidth(), modifiedBitmap.getHeight());
                break;
            case 5:
                nativeBitmapGrayingAVG(colorArray, modifiedBitmap.getWidth(), modifiedBitmap.getHeight());
                break;
            case 6:
                nativeBitmapGrayingWAVG(colorArray, modifiedBitmap.getWidth(), modifiedBitmap.getHeight());
                break;
            case 7:
                nativeBitmapBinary(colorArray, modifiedBitmap.getWidth(), modifiedBitmap.getHeight());
                break;
            case 8:
                nativeBitmapInverse(colorArray, modifiedBitmap.getWidth(), modifiedBitmap.getHeight());
                break;
            case 9:
                nativeBitmapEqualizationHist(colorArray, modifiedBitmap.getWidth(), modifiedBitmap.getHeight());
                break;
            case 10:
                nativeBitmapGaussianSmooth(colorArray, modifiedBitmap.getWidth(), modifiedBitmap.getHeight());
                break;
            case 11:
                nativeBitmapCanny(colorArray, modifiedBitmap.getWidth(), modifiedBitmap.getHeight());
                break;
            default:
                modified = false;
                break;
        }

        if (modified) {
            convertColorArrayToBitmap(colorArray, modifiedBitmap);
            mBinding.ivImage.setImageBitmap(modifiedBitmap);
        } else {
            mBinding.ivImage.setImageBitmap(mSourceBitmap);
        }
    }

    @Override
    public void onNothingSelected(AdapterView<?> adapterView) {
    }
}
