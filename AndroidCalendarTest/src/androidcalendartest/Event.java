package androidcalendartest;

import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONStringer;

public class Event {
	private String mName;
	private EventDate mTime;
	private String mPlace;
	private Group mOwner;
	
	public Event(JSONObject json) {
		this();
		try {
			mName = json.getString("name");
			mTime = new EventDate((JSONObject) json.get("time"));
			mPlace = json.getString("place");
			mOwner = new Group(json.getString("owner"));
		} catch (JSONException e) {
			e.printStackTrace();
			System.out.println("JSON error! Setting the event to default values.");
		}
	}
	
	public Event() {
		mName="";
		mTime = new EventDate();
		mPlace ="";
		mOwner=new Group("");
	}
	
	public Event(String name, EventDate time, String place, Group owner) {
		mName = name;
		mTime = time;
		mPlace = place;
		mOwner = owner;
	}
	
	public String getName() {
		return mName;
	}
	
	public EventDate getTime() {
		return mTime;
	}
	
	public String getPlace() {
		return mPlace;
	}
	
	public Group getOwner() {
		return mOwner;
	}
	
	public JSONObject toJSON() throws JSONException{
		try {
			JSONStringer stringer = new JSONStringer().object();
			stringer.key("name").value(mName);
			stringer.key("time").value(new JSONObject(mTime.toJSON().toString()));
			stringer.key("place").value(mPlace);
			stringer.key("owner").value(mOwner.getName());
			stringer.endObject();
			return new JSONObject(stringer.toString());
		} catch(JSONException e) {
			System.out.println("JSON Creation failed. returning empty JSON...");
			return new JSONObject();
		}
		
	}
}
