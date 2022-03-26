package com.jni

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.os.Message
import android.util.Log
import android.widget.TextView
import android.widget.Toast
import com.jni.databinding.ActivityMainBinding
import org.fmod.FMOD
import java.io.File

class MainActivity : AppCompatActivity() {

    val tag = "qglog"

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.sampleText.text = Test.stringFromJNI()

        Test.changeTxt()
        Log.i(tag, "newValue=" + Test.txt)

        Test.nativeCallJava()
        Test.opArray()


        Test.reference()
        Test.nativeCreatePerson().print()

        FMOD.init(this)
        val voiceFilePath = "file:///android_asset/test.mp3"
        Test.changeVoice(voiceFilePath)


        Test.dynamicRegister1()
        Test.dynamicRegister2("asdfasfasdf")

        Test.nativeWorkNotifyUi()
    }

    override fun onDestroy() {
        super.onDestroy()
        FMOD.close()
    }

}



























