package com.foo;
import android.content.Intent;
import org.qtproject.qt5.android.bindings.QtActivity;

class Listener
{
    public static native void openUri(String uri);
    // Notice the 'native' keyword
}

public class MyActivity extends QtActivity {
    public void onNewIntent(Intent i) {
        if (i.getAction() == Intent.VIEW) {
            Listener.openUri(i.getData().toString());
        }
        super.onNewIntent(i);
    }
}
