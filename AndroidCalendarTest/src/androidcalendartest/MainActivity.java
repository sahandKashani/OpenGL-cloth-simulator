package androidcalendartest;

import java.util.Calendar;

import com.example.androidcalendartest.R;

import android.net.Uri;
import android.os.Bundle;
import android.provider.CalendarContract;
import android.provider.CalendarContract.Events;
import android.app.Activity;
import android.content.ContentUris;
import android.content.Intent;
import android.view.Menu;
import android.view.View;

public class MainActivity extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }
    
    public void writeCalendar(View view) {
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
    
    public void getFromCalendar(View view) {
    	Calendar beginTime = Calendar.getInstance();
    	beginTime.set(2012, 0, 19, 7, 30);
    	Uri.Builder builder = CalendarContract.CONTENT_URI.buildUpon();
    	builder.appendPath("time");
    	ContentUris.appendId(builder, beginTime.getTimeInMillis());
    	Intent intent = new Intent(Intent.ACTION_VIEW)
    	    .setData(builder.build());
    	startActivity(intent);
    }
}
