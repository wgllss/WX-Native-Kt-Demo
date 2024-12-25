package com.wx.nativex.kt.demo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.TextView
import android.widget.Toast

class MainActivity : AppCompatActivity(), View.OnClickListener {

    private lateinit var sample_text: TextView
    val androidData = "十万伙计"

    /**
     * A native method that is implemented by the 'nativex' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun callJNI()
    external fun stringFromJNIwithParameter(str: String): String

    external fun callNativeCallJavaMethod()
    external fun callNativeCallJavaField()
    external fun callNativeWithCallBack(callBack: NativeCallBack)

    external fun dynamicRegisterCallBack(callBack: NativeCallBack)

    companion object {
        // Used to load the 'native_kt_demo' library on application startup.
        init {
            System.loadLibrary("native_kt_demo")
        }

        private fun logE(str: String) {
            android.util.Log.e("MainActivity", str)
        }
    }

    fun toast(str: String) {
        Toast.makeText(this, str, Toast.LENGTH_SHORT).show()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        sample_text = findViewById(R.id.sample_text)
        sample_text.text = stringFromJNI()


    }

    override fun onClick(v: View) {
        when (v.id) {
            R.id.btn_get_char -> toast(stringFromJNI())
            R.id.btn_voidr -> callJNI()
            R.id.btn_call_with_parameter -> toast(stringFromJNIwithParameter("AAAA"))
            R.id.btn_call_native_javar -> callNativeCallJavaMethod()
            R.id.btn_call_native_java_field -> {
                callNativeCallJavaField()
//                toast(androidData)
            }

            R.id.btn_call_native_java_with_callBack -> {
                callNativeWithCallBack(object : NativeCallBack {
                    override fun nmd(str: String) {
                        toast(str)
                    }
                })
            }

            R.id.btn_call_native_dynamic_register -> {
                dynamicRegisterCallBack(object : NativeCallBack {
                    override fun nmd(str: String) {
                        toast(str)
                    }
                })
            }

            else -> {}
        }
    }


}