package com.lzy.ndk;

import android.util.Log;

enum Signal {
    GREEN, YELLOW, RED
}

public class enumDemo {
    public static Signal color = Signal.RED;

    public static void main() {
        Log.d("4enum", "enum " + color.getClass().getName());
        switch (color) {
            case RED:
                color = Signal.GREEN;
                break;
            case YELLOW:
                color = Signal.RED;
                break;
            case GREEN:
                color = Signal.YELLOW;
                break;
        }
    }
}