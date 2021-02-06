package iii.calc;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity implements OnClickListener{
	double result=0.0, memory=0.0;
	String input="";
	Button b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,
	       bDot,bPlus,bMinus,bMul,bDiv,bDel,bAC,
	       bMS,bMR,bEqual;
	int op=0;
	TextView tv,tvSym;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		initAll();
	}
	
	void initAll()
	{
		tvSym = (TextView)findViewById(R.id.tvSymbol);
		tvSym.setText("");
		tv = (TextView) findViewById(R.id.txtLCD);
		b0 = (Button)findViewById(R.id.btn0);
		b1 = (Button)findViewById(R.id.btn1);
		b2 = (Button)findViewById(R.id.btn2);
		b3 = (Button)findViewById(R.id.btn3);
		b4 = (Button)findViewById(R.id.btn4);
		b5 = (Button)findViewById(R.id.btn5);
		b6 = (Button)findViewById(R.id.btn6);
		b7 = (Button)findViewById(R.id.btn7);
		b8 = (Button)findViewById(R.id.btn8);
		b9 = (Button)findViewById(R.id.btn9);
		bDot = (Button)findViewById(R.id.btnDot);
		bPlus = (Button)findViewById(R.id.btnPlus);
		bMinus = (Button)findViewById(R.id.btnMinus);
		bMul = (Button)findViewById(R.id.btnMul);
		bDiv = (Button)findViewById(R.id.btnDiv);
		bMS = (Button)findViewById(R.id.btnMemStore);
		bMR = (Button)findViewById(R.id.btnMemRecall);
		bDel = (Button)findViewById(R.id.btnDel);
		bAC = (Button)findViewById(R.id.btnAC);
		bEqual = (Button)findViewById(R.id.btnEqual);
		
		b0.setOnClickListener(this);
		b1.setOnClickListener(this);
		b2.setOnClickListener(this);
		b3.setOnClickListener(this);
		b4.setOnClickListener(this);
		b5.setOnClickListener(this);
		b6.setOnClickListener(this);
		b7.setOnClickListener(this);
		b8.setOnClickListener(this);
		b9.setOnClickListener(this);
		bDot.setOnClickListener(this);
		bPlus.setOnClickListener(this);
		bMinus.setOnClickListener(this);
		bDiv.setOnClickListener(this);
		bMul.setOnClickListener(this);
		bMS.setOnClickListener(this);
		bMR.setOnClickListener(this);
		bAC.setOnClickListener(this);
		bDel.setOnClickListener(this);
		bEqual.setOnClickListener(this);
		bDel.setOnClickListener(this);
	}
	
	void showResult()
	{
		tv.setText(Double.toString(result));
	}
	

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId())
		{
			case R.id.btn0:
					if (input.length() > 0)
					{
						input += "0";
						tv.setText(input);
					}
					break;
			case R.id.btn1:
					input += "1";
					tv.setText(input);
				break;
			case R.id.btn2:
					input +="2";
					tv.setText(input);
				break;
			case R.id.btn3:
					input+="3";
					tv.setText(input);
				break;
			case R.id.btn4:
					input +="4";
					tv.setText(input);
				break;
			case R.id.btn5:
					input +="5";
					tv.setText(input);
				break;
			case R.id.btn6:
					input +="6";
					tv.setText(input);
				break;
			case R.id.btn7:
					input +="7";
					tv.setText(input);
				break;
			case R.id.btn8:
				input +="8";
				tv.setText(input);
				break;
			case R.id.btn9:
				input +="9";
				tv.setText(input);
				break;
			case R.id.btnDot:
				if (!input.contains("."))
				{
					input+=".";
					tv.setText(input);
				}
				break;
			case R.id.btnPlus:
				tvSym.setText("+");
				op = 1;
				if (input.length()>0)
					if (result == 0.0)
						result=Double.parseDouble(tv.getText().toString());
					else
						result+=Double.parseDouble(tv.getText().toString());
				showResult();	
				input="";
				break;
			case R.id.btnMinus:
				tvSym.setText("-");
				op = 2;
				if (input.length()>0)
					if (result == 0.0)
						result=Double.parseDouble(tv.getText().toString());
					else
						result-=Double.parseDouble(tv.getText().toString());
				showResult();	
				input="";
				break;
			case R.id.btnDiv:
				tvSym.setText("/");
				switch (op)
				{
				case 1:
					if (input.length()>0)
						result += Double.parseDouble(input);
					break;
				case 2:
					if (input.length()>0)
						result -= Double.parseDouble(input);
					break;
				case 3:
					if (input.length()>0)
						result /= Double.parseDouble(input);
					break;
				case 4:
					if (input.length()>0)
						result *= Double.parseDouble(input);
					break;
				}
				op = 3;
				if (input.length()>0)
					if (result == 0.0)
						result=Double.parseDouble(tv.getText().toString());
					else
						result/=Double.parseDouble(tv.getText().toString());

				showResult();	
				input="";
				break;
			case R.id.btnMul:
				tvSym.setText("x");
				switch (op)
				{
				case 1:
					if (input.length()>0)
						result += Double.parseDouble(input);
					break;
				case 2:
					if (input.length()>0)
						result -= Double.parseDouble(input);
					break;
				case 3:
					if (input.length()>0)
						result /= Double.parseDouble(input);
					break;
				case 4:
					if (input.length()>0)
						result *= Double.parseDouble(input);
					break;
				}
				op = 4;
				if (input.length()>0)
					if (result == 0.0)
						result=Double.parseDouble(tv.getText().toString());
					else
						result*=Double.parseDouble(tv.getText().toString());
				showResult();	
				input="";
				break;
			case R.id.btnMemStore:
				memory = Double.parseDouble(input);
				tvSym.setText(tvSym.getText().toString() + "  MS");
				break;
			case R.id.btnMemRecall:
				input = Double.toString(memory);
				tv.setText(input);
				break;
			case R.id.btnDel:
				if (input.length()>0)
				{
					input = input.substring(0, input.length()-1);
					if (input.length()==0)
						tv.setText("0");
					else
						tv.setText(input);
				}
				break;
			case R.id.btnEqual:
				switch (op)
				{
				case 1:
					if (input.length()>0)
						result += Double.parseDouble(input);
					break;
				case 2:
					if (input.length()>0)
						result -= Double.parseDouble(input);
					break;
				case 3:
					if (input.length()>0)
						result /= Double.parseDouble(input);
					break;
				case 4:
					if (input.length()>0)
						result *= Double.parseDouble(input);
					break;
				}
				showResult();
				tvSym.setText("");
				input = "";
				op = 0;
				break;

			case R.id.btnAC:
				tvSym.setText("");
				tv.setText("0");
				input = "";
				result = 0.0;
				op =0;
				break;
		}
	}
}
