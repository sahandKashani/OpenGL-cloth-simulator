package androidcalendartest;

import java.util.Calendar;

import org.json.JSONException;
import org.json.JSONObject;

import com.example.androidcalendartest.R;

import android.os.Bundle;
import android.provider.CalendarContract;
import android.provider.CalendarContract.Events;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.View;
import android.widget.DatePicker;
import android.widget.TextView;

public class EventDetailsActivity extends Activity {
	
	private final static String OWNER_STRING = "Owner: ";

	TextView mEventTitle;
	TextView mPlace;
	TextView mOwner;
	TextView mTime;
	
	Event mEvent;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_event_details);
        JSONObject json;
        try {
            json = new JSONObject(this.getIntent().getStringExtra("event"));
        } catch(JSONException e) {
        	System.out.println("JSON error! Setting the JSON to an empty JSONObject.");
        	json = new JSONObject();
        }
        mEvent = new Event(json);
        
        mEventTitle = (TextView) findViewById(R.id.eventTitle);
        mPlace = (TextView) findViewById(R.id.place);
        mOwner = (TextView) findViewById(R.id.owner);
        mTime = (TextView) findViewById(R.id.time);
        
        
        mEventTitle.setText(mEvent.getName());
        mPlace.setText(mEvent.getPlace());
        mOwner.setText(OWNER_STRING+mEvent.getOwner().getName());
        mTime.setText(mEvent.getTime().toString());
        
        
        
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_event_details, menu);
        return true;
    }
    
    public void exportToCalendar(View view) {
    	Calendar beginTime = Calendar.getInstance();
    	beginTime.set(mEvent.getTime().getBegin().getYear(), 
    			mEvent.getTime().getBegin().getMonth()-1,
    			mEvent.getTime().getBegin().getDay(),
    			mEvent.getTime().getBegin().getHour(),
    			mEvent.getTime().getBegin().getMinute());
    	Calendar endTime = Calendar.getInstance();
    	endTime.set(mEvent.getTime().getEnd().getYear(), 
    			mEvent.getTime().getEnd().getMonth()-1,
    			mEvent.getTime().getEnd().getDay(),
    			mEvent.getTime().getEnd().getHour(),
    			mEvent.getTime().getEnd().getMinute());
    	Intent intent = new Intent(Intent.ACTION_INSERT)
    	        .setData(Events.CONTENT_URI)
    	        .putExtra(CalendarContract.EXTRA_EVENT_BEGIN_TIME, beginTime.getTimeInMillis())
    	        .putExtra(CalendarContract.EXTRA_EVENT_END_TIME, endTime.getTimeInMillis())
    	        .putExtra(Events.TITLE, mEventTitle.getText())
    	        .putExtra(Events.DESCRIPTION, mEvent.getOwner().getName())
    	        .putExtra(Events.EVENT_LOCATION, mPlace.getText())
    	        .putExtra(Events.AVAILABILITY, Events.AVAILABILITY_BUSY);
    	if(mEvent.getTime().isAllDay()) {
    		intent.putExtra(Events.ALL_DAY, true);
    	}
    	startActivity(intent);
    }
}
