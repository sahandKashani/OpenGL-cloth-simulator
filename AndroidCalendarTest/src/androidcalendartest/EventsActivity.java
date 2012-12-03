package androidcalendartest;

import org.json.JSONException;

import com.example.androidcalendartest.R;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.view.Menu;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.TextView;

public class EventsActivity extends Activity {

	LinearLayout mEventsLayout;
	Event[] mEvents;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_events);
        
        Event event1 = new Event("Compiler Construction revision",new EventDate(false,new Clock(2012,12,25,16,00),new Clock(2012,12,25,17,00)),
    			"INM202", new Group("SemaphoreLovers"));
        Event event2 = new Event("Souper de faculté",new EventDate(false,new Clock(2012,12,25,18,30),new Clock(2012,12,26,19,30)),
    			"Hall BC", new Group("CLIC"));
        Event event3 = new Event("Balélec",new EventDate(true,new Clock(2013,5,10,0,0),new Clock(2012,12,01,0,0)),	
    			"Esplanade", new Group("Balelec"));
        
        mEvents = new Event[6];
        mEvents[0] = event1; mEvents[1] = event2; mEvents[2] = event3;
        mEvents[3] = event1; mEvents[4] = event2; mEvents[5] = event3;
        
        mEventsLayout = (LinearLayout) this.findViewById(R.id.events);
        for(int i=0; i<mEvents.length; i++) {
	        LinearLayout oneEventLayout = (LinearLayout) getLayoutInflater().inflate(R.layout.activity_events_one_event, null);
	        TextView title = (TextView) oneEventLayout.findViewById(R.id.eventTitle);
	        title.setText(mEvents[i].getName());
	        TextView place = (TextView) oneEventLayout.findViewById(R.id.place);
	        place.setText(mEvents[i].getPlace());
	        TextView time = (TextView) oneEventLayout.findViewById(R.id.time);
	        time.setText(mEvents[i].getTime().toString());
	        mEventsLayout.addView(oneEventLayout);
	    }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_events, menu);
        return true;
    }
    
    public void openDetails(View view) {
    	boolean ok=false;
    	Event event = null;
    	for(int i=0; i<mEvents.length && !ok; i++) {
    		TextView title = (TextView) view.findViewById(R.id.eventTitle);
    		if(mEvents[i].getName().equals(title.getText())) {
    			ok = true;
    			event = mEvents[i];
    		}
    	}
    	
    	Intent intent = new Intent(this.getApplicationContext(), EventDetailsActivity.class);
    	try {
    		intent.putExtra("event", event.toJSON().toString());
    	} catch (JSONException e) {
    		e.printStackTrace();
    	}
    	startActivity(intent);

    }
    
}
