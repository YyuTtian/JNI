package com.jni

import android.os.Handler
import android.os.Looper
import android.widget.Toast

object Test {

    init {
        System.loadLibrary("jni_test")
    }

    val txt = "qgl"

    external fun stringFromJNI(): String

    external fun changeTxt()

    external fun nativeCallJava()

    // Native 调 Java
    fun add(num1: Int, num2: Int): Int {
        return num1 + num2
    }

    external fun nativeOpArrays(intArray: IntArray, stringArray: Array<String>)
    fun opArray() {
        val intArray = intArrayOf(1, 2, 3, 4, 5, 6)
        val stringArray = arrayOf("1", "2", "3", "4", "5", "6")
        nativeOpArrays(intArray, stringArray)

        intArray.forEach {
            println("qglog Java intArray=$it")
        }

        stringArray.forEach {
            println("qglog Java stringArray=$it")
        }
    }

    external fun reference()

    external fun nativeCreatePerson(): Person

    external fun changeVoice(path: String)
    fun playerEnd(msg: String) {
        Toast.makeText(App.context, msg, Toast.LENGTH_SHORT).show()
    }

    external fun dynamicRegister1()
    external fun dynamicRegister2(name: String): Int

    external fun nativeWorkNotifyUi(): Int

    fun msg(msg: String) {
        println("qglog msg=$msg")
    }
}