package androidcalendartest;

import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONStringer;

public class Clock {
	private int mYear;
	private int mMonth;
	private int mDay;
	private int mHour;
	private int mMinute;
	
	public Clock(int year, int month, int day, int hour, int minute) {
		mYear = year;
		mMonth = month;
		mDay = day;
		mHour = hour;
		mMinute = minute;
	}
	
	public Clock() {
		mYear = 0; mMonth = 0; mDay = 0; mHour = 0; mMinute = 0;
	}

	public Clock(JSONObject json) throws JSONException {
		mYear = json.getInt("year");
		mMonth = json.getInt("month");
		mDay = json.getInt("day");
		mHour = json.getInt("hour");
		mMinute = json.getInt("minute");
	}

	public int getYear() {
		return mYear;
	}
	
	public int getMonth() {
		return mMonth;
	}
	
	public int getDay() {
		return mDay;
	}
	
	public int getHour() {
		return mHour;
	}
	
	public int getMinute() {
		return mMinute;
	}
	
	public JSONObject toJSON() throws JSONException {
		JSONStringer stringer = new JSONStringer().object();
		stringer.key("year").value(mYear);
		stringer.key("month").value(mMonth);
		stringer.key("day").value(mDay);
		stringer.key("hour").value(mHour);
		stringer.key("minute").value(mMinute);
		return new JSONObject(stringer.endObject().toString());
	}
}
