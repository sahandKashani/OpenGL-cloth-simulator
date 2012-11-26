package androidcalendartest;

import com.example.androidcalendartest.R;
import com.example.androidcalendartest.R.layout;
import com.example.androidcalendartest.R.menu;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class EventDetailsActivity extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_event_details);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_event_details, menu);
        return true;
    }
}
