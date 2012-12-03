package androidcalendartest;

import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONStringer;

public class EventDate {
	private boolean mAllDay;
	
	private Clock mBegin;
	private Clock mEnd;
	
	public EventDate(boolean allDay, Clock begin, Clock end) {
		mAllDay = allDay;
		mBegin = begin;
		mEnd = end;
	}
	
	public EventDate(JSONObject json) throws JSONException {
		mAllDay = json.getBoolean("allDay");
		mBegin = new Clock((JSONObject) json.get("begin"));
		mEnd = new Clock((JSONObject) json.get("end"));
	}
	
	public EventDate() {
		mAllDay = true;
		mBegin = new Clock();
		mEnd = new Clock();
	}

	public boolean isAllDay() {
		return mAllDay;
	}
	
	public Clock getBegin() {
		return mBegin;
	}
	
	public Clock getEnd() {
		return mEnd;
	}
	
	public JSONObject toJSON() throws JSONException {
		JSONStringer stringer = new JSONStringer().object();
		stringer.key("allDay").value(mAllDay);
		stringer.key("begin").value(mBegin.toJSON());
		stringer.key("end").value(mEnd.toJSON());
		stringer.endObject();
		return new JSONObject(stringer.toString());
	}
	
	public String toString() {
		if(mAllDay) {
			return mBegin.getDay()+"."+mBegin.getMonth()+"."+mBegin.getYear()+" - All day";
		} else if(mBegin.getDay() == mEnd.getDay() &&
			mBegin.getMonth() == mEnd.getMonth() &&
			mBegin.getYear() == mEnd.getYear()) {
			return mBegin.getDay()+"."+mBegin.getMonth()+"."+mBegin.getYear()+": "+mBegin.getHour()+"h"+(mBegin.getMinute()<10 ? "0" : "")+mBegin.getMinute()+" - "+mEnd.getHour()+"h"+(mEnd.getMinute()<10 ? "0" : "")+mEnd.getMinute();
		}
		return mBegin.getDay()+"."+mBegin.getMonth()+"."+mBegin.getYear()+" ("+mBegin.getHour()+"h"+(mBegin.getMinute()<10 ? "0" : "")+mBegin.getMinute()+") - "
				+mEnd.getDay()+"."+mEnd.getMonth()+"."+mEnd.getYear()+" ("+mEnd.getHour()+"h"+(mEnd.getMinute()<10 ? "0" : "")+mEnd.getMinute()+")";
	}
	
}
