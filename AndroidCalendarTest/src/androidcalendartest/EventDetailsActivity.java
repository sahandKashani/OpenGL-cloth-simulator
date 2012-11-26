package androidcalendartest;

import java.util.Calendar;

import com.example.androidcalendartest.R;
import com.example.androidcalendartest.R.layout;
import com.example.androidcalendartest.R.menu;

import android.os.Bundle;
import android.provider.CalendarContract;
import android.provider.CalendarContract.Events;
import android.app.Activity;
import android.app.Dialog;
import android.content.Intent;
import android.view.Menu;
import android.view.View;

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
    
    public void exportToCalendar(View view) {
    	Calendar beginTime = Calendar.getInstance();
    	beginTime.set(2012, 0, 19, 7, 30);
    	Calendar endTime = Calendar.getInstance();
    	endTime.set(2012, 0, 19, 8, 30);
    	Intent intent = new Intent(Intent.ACTION_INSERT)
    	        .setData(Events.CONTENT_URI)
    	        .putExtra(CalendarContract.EXTRA_EVENT_BEGIN_TIME, beginTime.getTimeInMillis())
    	        .putExtra(CalendarContract.EXTRA_EVENT_END_TIME, endTime.getTimeInMillis())
    	        .putExtra(Events.TITLE, "Yoga")
    	        .putExtra(Events.DESCRIPTION, "Group class")
    	        .putExtra(Events.EVENT_LOCATION, "The gym")
    	        .putExtra(Events.AVAILABILITY, Events.AVAILABILITY_BUSY)
    	        .putExtra(Intent.EXTRA_EMAIL, "rowan@example.com,trevor@example.com");
    	startActivity(intent);
    }
}
