package androidcalendartest;

import java.util.Calendar;

import com.example.androidcalendartest.R;
import android.os.Bundle;
import android.app.Activity;
import android.app.DatePickerDialog;
import android.app.TimePickerDialog;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.TimePicker;

public class EventEditActivity extends Activity {

	// private DatePicker mDPicker;
	private Button mBeginDateField;
	private Button mBeginTimeField;
	
	private int mBeginYear;
	private int mBeginMonth;
	private int mBeginDay;
	
	private int mBeginHour;
	private int mBeginMinute;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_event_edit);

		Calendar calendar = Calendar.getInstance();
		mBeginYear = calendar.get(Calendar.YEAR);
		mBeginMonth = calendar.get(Calendar.MONTH);
		mBeginDay = calendar.get(Calendar.DAY_OF_MONTH);

		mBeginDateField = (Button) this
				.findViewById(R.id.edit_event_begin_date);
		
		mBeginTimeField = (Button) this.findViewById(R.id.edit_event_begin_time);

		updateDateDisplay(); updateTimeDisplay();
	}

	private DatePickerDialog.OnDateSetListener datePickerListener = new DatePickerDialog.OnDateSetListener() {

		// when dialog box is closed, below method will be called.
		public void onDateSet(DatePicker view, int selectedYear,
				int selectedMonth, int selectedDay) {
			mBeginYear = selectedYear;
			mBeginMonth = selectedMonth;
			mBeginDay = selectedDay;

			// set selected date into textview
			mBeginDateField.setText(new StringBuilder().append(mBeginDay)
					.append(".").append(mBeginMonth + 1).append(".")
					.append(mBeginYear).append(" "));

			// set selected date into datepicker also
			view.init(mBeginYear, mBeginMonth, mBeginDay, null);

		}
	};

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.activity_event_edit, menu);
		return true;
	}

	public void enterDate(View view) {
		new DatePickerDialog(this, datePickerListener, mBeginYear, mBeginMonth,
				mBeginDay).show();
	}

	private void updateDateDisplay() {
		this.mBeginDateField.setText(new StringBuilder()
				// Month is 0 based so add 1
				.append(mBeginDay).append(".").append(mBeginMonth + 1)
				.append(".").append(mBeginYear).append(" "));
	}

	private TimePickerDialog.OnTimeSetListener timePickerListener = new TimePickerDialog.OnTimeSetListener() {

		@Override
		public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
			mBeginHour = hourOfDay;
			mBeginMinute = minute;

			// set selected date into textview
			updateTimeDisplay();

			// set selected date into datepicker also
			view.setCurrentHour(mBeginHour);
			view.setCurrentMinute(mBeginMinute);
		}
	};

	public void enterTime(View view) {
		new TimePickerDialog(this, timePickerListener, mBeginHour, mBeginMinute,false).show();
	}

	private void updateTimeDisplay() {
		mBeginTimeField.setText(new StringBuilder().append((mBeginHour<10 ? "0" : "")+mBeginHour)
				.append("h").append((mBeginMinute<10 ? "0" : "")+mBeginMinute).append(" "));
	}
	
	public void submit(View view) {
		
		//Getting title
		EditText titleField = (EditText) this.findViewById(R.id.edit_event_title);
		String title = titleField.getText().toString();
		
		//Getting begin clock
		Clock beginClock = new Clock(mBeginYear, mBeginMonth, mBeginDay, mBeginHour, mBeginMinute);
		
		
		
	}

}
