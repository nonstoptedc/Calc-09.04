#include "MainForm.h"
#include "mathfunc.h"
#include <string>

using namespace System;
using namespace System::Windows::Forms;

void main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	SimpleCCalculator::MainForm form;
	Application::Run(% form);
}

void button_action(System::Windows::Forms::TextBox^ textbox1, System::Windows::Forms::TextBox^ textbox2, char calculation_symbol) {
	if (textbox1->Text != "E") {
		if (!String::IsNullOrWhiteSpace(textbox2->Text)) {
			if (textbox2->Text->EndsWith("+") || textbox2->Text->EndsWith("-") || textbox2->Text->EndsWith("*") || textbox2->Text->EndsWith("/")) {
				if (textbox1->Text == "0") {
					textbox2->Text = textbox2->Text->Substring(0, textbox2->Text->Length - 1);
					textbox2->Text += gcnew String(&calculation_symbol);
				}
				else {
					calculate(textbox1, textbox2);
					textbox2->Text = textbox1->Text;
					textbox1->Text = "0";
					textbox2->Text += gcnew String(&calculation_symbol);
				}
			}
		}
		else if (textbox1->Text != "0") {
			if (textbox1->Text->EndsWith(","))
				textbox2->Text = textbox1->Text->Substring(0, textbox1->Text->Length - 1);
			else
				textbox2->Text = textbox1->Text;
			textbox1->Text = "0";
			textbox2->Text += gcnew String(&calculation_symbol);
		}
	}
}


void calculate(System::Windows::Forms::TextBox^ textbox1, System::Windows::Forms::TextBox^ textbox2) {
	try {
		if (textbox2->Text->Contains("+") || textbox2->Text->Contains("-") || textbox2->Text->Contains("*") || textbox2->Text->Contains("/")) {
			String^ calculation_type = (textbox2->Text->Substring(textbox2->Text->Length - 1, 1));
			textbox2->Text = textbox2->Text->Substring(0, textbox2->Text->Length - 1);
			double a;
			double::TryParse(textbox2->Text, a);
			double b;
			double::TryParse(textbox1->Text, b);
			char ct = calculation_type[0];
			double result;
			switch (ct) {
			case '+':
				result = (a + b);
				break;
			case '-':
				result = (a - b);
				break;
			case '*':
				result = (a * b);
				break;
			case '/':
				if (b == 0)
					throw "0";
				result = (a / b);
				break;
			}
			textbox1->Text = result.ToString("F11");
			textbox1_format(textbox1);
			textbox2->Clear();
		}
		else if (!String::IsNullOrWhiteSpace(textbox2->Text)) {
			textbox1->Text = textbox2->Text;
			textbox2->Clear();
		}
	}
	catch (...) {
		textbox2->Clear();
		textbox1->Text = "E";
		return;
	}
}

void calculate_percent(System::Windows::Forms::TextBox^ textbox1, System::Windows::Forms::TextBox^ textbox2) {
	double temp;
	double::TryParse(textbox1->Text, temp);
	if (String::IsNullOrWhiteSpace(textbox2->Text))
		textbox1->Text = "0";
	else if (textbox2->Text->EndsWith("+") || textbox2->Text->EndsWith("-")) {
		double base;
		double::TryParse(textbox2->Text->Substring(0, textbox2->Text->Length - 1), base);
		textbox1->Text = (temp * base / 100).ToString();
	}
	else {
		textbox1->Text = (temp / 100).ToString();
	}
	return;

}

void calculate_one(System::Windows::Forms::TextBox^ textbox1, double (*calculation_function) (double)) {
	try {
		if (textbox1->Text == "E")
			throw "E";
		double temp;
		double::TryParse(textbox1->Text, temp);
		if (calculation_function == inverse && textbox1->Text == "0")
			throw 0;
		double result = calculation_function(temp);
		textbox1->Text = result.ToString("F11");
		textbox1_format(textbox1);
	}
	catch (...) {
		textbox1->Text = "E";
		return;
	}
}

void append_number(System::Windows::Forms::TextBox^ textbox1, double number) {
	if (textbox1->Text->Length < 12 && textbox1->Text != "E") {
		if (textbox1->Text != "0")
			textbox1->Text += number;
		else
			textbox1->Text = number.ToString();
	}
}

void backspace(System::Windows::Forms::TextBox^ textbox1) {
	textbox1->Text = textbox1->Text->Substring(0, textbox1->Text->Length - 1);
	if (String::IsNullOrWhiteSpace(textbox1->Text))
		textbox1->Text = "0";
}

void textbox1_format(System::Windows::Forms::TextBox^ textbox1) {
	while (textbox1->Text[textbox1->Text->Length - 1] == '0' || textbox1->Text[textbox1->Text->Length - 1] == ',') {
		textbox1->Text = textbox1->Text->Substring(0, textbox1->Text->Length - 1);
		if (!textbox1->Text->Contains(","))
			break;
	}
	if (textbox1->Text->Length > 12)
		if (textbox1->Text->Contains(","))
			textbox1->Text = textbox1->Text->Substring(0, 12);
		else
			throw 1;
}