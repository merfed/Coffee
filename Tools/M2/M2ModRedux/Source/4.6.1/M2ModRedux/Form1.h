#pragma once

#define M2Filter L"M2 Files|*.m2|All Files|*.*"
#define M2IFilter L"M2I Files|*.m2i|All Files|*.*"

namespace M2ModRedux
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::ToolTip^  toolTip1;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabExport;
	private: System::Windows::Forms::Panel^  panelImputM2Exp;
	private: System::Windows::Forms::TextBox^  textBoxInputM2Exp;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  buttonInputM2ExpBrowse;
	private: System::Windows::Forms::Panel^  panelOutputM2I;
	private: System::Windows::Forms::Button^  buttonOutputM2IBrowse;
	private: System::Windows::Forms::TextBox^  textBoxOutputM2I;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::TabPage^  tabImport;
	private: System::Windows::Forms::Panel^  panelInputM2Import;
	private: System::Windows::Forms::TextBox^  textBoxInputM2Imp;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Button^  buttonInputM2ImpBrowse;
	private: System::Windows::Forms::Panel^  panelInputM2I;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  buttonInputM2IBrowse;
	private: System::Windows::Forms::TextBox^  textBoxInputM2I;
	private: System::Windows::Forms::Panel^  panelOutputM2;
	private: System::Windows::Forms::TextBox^  textBoxOutputM2;
	private: System::Windows::Forms::Button^  buttonOutputM2Browse;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Panel^  panelImportCb;
	private: System::Windows::Forms::CheckBox^  checkBoxMergeCameras;
	private: System::Windows::Forms::CheckBox^  checkBoxMergeAttachments;
	private: System::Windows::Forms::CheckBox^  checkBoxMergeBones;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  labelSatus;
	private: System::Windows::Forms::Button^  buttonGo;
	private: System::ComponentModel::IContainer^  components;


#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// Required method for Designer support - do not modify
			 /// the contents of this method with the code editor.
			 /// </summary>
			 void InitializeComponent(void)
			 {
				 this->components = (gcnew System::ComponentModel::Container());
				 System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
				 this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
				 this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
				 this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
				 this->label1 = (gcnew System::Windows::Forms::Label());
				 this->buttonInputM2ExpBrowse = (gcnew System::Windows::Forms::Button());
				 this->buttonOutputM2IBrowse = (gcnew System::Windows::Forms::Button());
				 this->label7 = (gcnew System::Windows::Forms::Label());
				 this->label3 = (gcnew System::Windows::Forms::Label());
				 this->buttonInputM2IBrowse = (gcnew System::Windows::Forms::Button());
				 this->buttonOutputM2Browse = (gcnew System::Windows::Forms::Button());
				 this->label2 = (gcnew System::Windows::Forms::Label());
				 this->checkBoxMergeCameras = (gcnew System::Windows::Forms::CheckBox());
				 this->checkBoxMergeAttachments = (gcnew System::Windows::Forms::CheckBox());
				 this->checkBoxMergeBones = (gcnew System::Windows::Forms::CheckBox());
				 this->buttonGo = (gcnew System::Windows::Forms::Button());
				 this->label5 = (gcnew System::Windows::Forms::Label());
				 this->buttonInputM2ImpBrowse = (gcnew System::Windows::Forms::Button());
				 this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
				 this->tabExport = (gcnew System::Windows::Forms::TabPage());
				 this->panelImputM2Exp = (gcnew System::Windows::Forms::Panel());
				 this->textBoxInputM2Exp = (gcnew System::Windows::Forms::TextBox());
				 this->panelOutputM2I = (gcnew System::Windows::Forms::Panel());
				 this->textBoxOutputM2I = (gcnew System::Windows::Forms::TextBox());
				 this->tabImport = (gcnew System::Windows::Forms::TabPage());
				 this->panelInputM2Import = (gcnew System::Windows::Forms::Panel());
				 this->textBoxInputM2Imp = (gcnew System::Windows::Forms::TextBox());
				 this->panelInputM2I = (gcnew System::Windows::Forms::Panel());
				 this->textBoxInputM2I = (gcnew System::Windows::Forms::TextBox());
				 this->panelOutputM2 = (gcnew System::Windows::Forms::Panel());
				 this->textBoxOutputM2 = (gcnew System::Windows::Forms::TextBox());
				 this->panelImportCb = (gcnew System::Windows::Forms::Panel());
				 this->label4 = (gcnew System::Windows::Forms::Label());
				 this->labelSatus = (gcnew System::Windows::Forms::Label());
				 this->tabControl1->SuspendLayout();
				 this->tabExport->SuspendLayout();
				 this->panelImputM2Exp->SuspendLayout();
				 this->panelOutputM2I->SuspendLayout();
				 this->tabImport->SuspendLayout();
				 this->panelInputM2Import->SuspendLayout();
				 this->panelInputM2I->SuspendLayout();
				 this->panelOutputM2->SuspendLayout();
				 this->panelImportCb->SuspendLayout();
				 this->SuspendLayout();
				 // 
				 // openFileDialog1
				 // 
				 this->openFileDialog1->FileName = L"openFileDialog1";
				 // 
				 // toolTip1
				 // 
				 this->toolTip1->AutoPopDelay = 5000;
				 this->toolTip1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(38)), static_cast<System::Int32>(static_cast<System::Byte>(16)),
					 static_cast<System::Int32>(static_cast<System::Byte>(4)));
				 this->toolTip1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(242)), static_cast<System::Int32>(static_cast<System::Byte>(94)),
					 static_cast<System::Int32>(static_cast<System::Byte>(131)));
				 this->toolTip1->InitialDelay = 1;
				 this->toolTip1->ReshowDelay = 1;
				 this->toolTip1->UseAnimation = false;
				 this->toolTip1->UseFading = false;
				 // 
				 // label1
				 // 
				 this->label1->AutoSize = true;
				 this->label1->Location = System::Drawing::Point(20, 7);
				 this->label1->Name = L"label1";
				 this->label1->Size = System::Drawing::Size(46, 13);
				 this->label1->TabIndex = 3;
				 this->label1->Text = L"InputM2";
				 this->toolTip1->SetToolTip(this->label1, L"Required. Select an M2 for M2Mod to work with.");
				 // 
				 // buttonInputM2ExpBrowse
				 // 
				 this->buttonInputM2ExpBrowse->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
				 this->buttonInputM2ExpBrowse->Location = System::Drawing::Point(452, 3);
				 this->buttonInputM2ExpBrowse->Name = L"buttonInputM2ExpBrowse";
				 this->buttonInputM2ExpBrowse->Size = System::Drawing::Size(75, 20);
				 this->buttonInputM2ExpBrowse->TabIndex = 5;
				 this->buttonInputM2ExpBrowse->Text = L"...";
				 this->toolTip1->SetToolTip(this->buttonInputM2ExpBrowse, L"Browse...");
				 this->buttonInputM2ExpBrowse->UseVisualStyleBackColor = true;
				 this->buttonInputM2ExpBrowse->Click += gcnew System::EventHandler(this, &Form1::buttonInputM2Browse_Click);
				 // 
				 // buttonOutputM2IBrowse
				 // 
				 this->buttonOutputM2IBrowse->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
				 this->buttonOutputM2IBrowse->Location = System::Drawing::Point(453, 3);
				 this->buttonOutputM2IBrowse->Name = L"buttonOutputM2IBrowse";
				 this->buttonOutputM2IBrowse->Size = System::Drawing::Size(75, 20);
				 this->buttonOutputM2IBrowse->TabIndex = 8;
				 this->buttonOutputM2IBrowse->Text = L"...";
				 this->toolTip1->SetToolTip(this->buttonOutputM2IBrowse, L"Browse...");
				 this->buttonOutputM2IBrowse->UseVisualStyleBackColor = true;
				 this->buttonOutputM2IBrowse->Click += gcnew System::EventHandler(this, &Form1::buttonOutputM2IBrowse_Click);
				 // 
				 // label7
				 // 
				 this->label7->AutoSize = true;
				 this->label7->Location = System::Drawing::Point(10, 6);
				 this->label7->Name = L"label7";
				 this->label7->Size = System::Drawing::Size(57, 13);
				 this->label7->TabIndex = 6;
				 this->label7->Text = L"OutputM2I";
				 this->toolTip1->SetToolTip(this->label7, L"Optional. If set, M2Mod will convert InputM2 to an M2I and save it here.");
				 // 
				 // label3
				 // 
				 this->label3->AutoSize = true;
				 this->label3->Location = System::Drawing::Point(17, 8);
				 this->label3->Name = L"label3";
				 this->label3->Size = System::Drawing::Size(49, 13);
				 this->label3->TabIndex = 9;
				 this->label3->Text = L"InputM2I";
				 this->toolTip1->SetToolTip(this->label3, L"Optional. If set, M2Mod will merge InputM2 with InputM2I to create a modified M2 "
					 L"which will be saved to OutputM2.");
				 // 
				 // buttonInputM2IBrowse
				 // 
				 this->buttonInputM2IBrowse->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
				 this->buttonInputM2IBrowse->Location = System::Drawing::Point(452, 5);
				 this->buttonInputM2IBrowse->Name = L"buttonInputM2IBrowse";
				 this->buttonInputM2IBrowse->Size = System::Drawing::Size(75, 20);
				 this->buttonInputM2IBrowse->TabIndex = 11;
				 this->buttonInputM2IBrowse->Text = L"...";
				 this->toolTip1->SetToolTip(this->buttonInputM2IBrowse, L"Browse...");
				 this->buttonInputM2IBrowse->UseVisualStyleBackColor = true;
				 this->buttonInputM2IBrowse->Click += gcnew System::EventHandler(this, &Form1::buttonInputM2IBrowse_Click);
				 // 
				 // buttonOutputM2Browse
				 // 
				 this->buttonOutputM2Browse->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
				 this->buttonOutputM2Browse->Location = System::Drawing::Point(452, 2);
				 this->buttonOutputM2Browse->Name = L"buttonOutputM2Browse";
				 this->buttonOutputM2Browse->Size = System::Drawing::Size(75, 20);
				 this->buttonOutputM2Browse->TabIndex = 14;
				 this->buttonOutputM2Browse->Text = L"...";
				 this->toolTip1->SetToolTip(this->buttonOutputM2Browse, L"Browse...");
				 this->buttonOutputM2Browse->UseVisualStyleBackColor = true;
				 this->buttonOutputM2Browse->Click += gcnew System::EventHandler(this, &Form1::buttonOutputM2Browse_Click);
				 // 
				 // label2
				 // 
				 this->label2->AutoSize = true;
				 this->label2->Location = System::Drawing::Point(12, 5);
				 this->label2->Name = L"label2";
				 this->label2->Size = System::Drawing::Size(54, 13);
				 this->label2->TabIndex = 12;
				 this->label2->Text = L"OutputM2";
				 this->toolTip1->SetToolTip(this->label2, L"Optional. If set, this is where M2Mod will save the modified M2.");
				 // 
				 // checkBoxMergeCameras
				 // 
				 this->checkBoxMergeCameras->AutoSize = true;
				 this->checkBoxMergeCameras->Checked = true;
				 this->checkBoxMergeCameras->CheckState = System::Windows::Forms::CheckState::Checked;
				 this->checkBoxMergeCameras->Location = System::Drawing::Point(72, 46);
				 this->checkBoxMergeCameras->Name = L"checkBoxMergeCameras";
				 this->checkBoxMergeCameras->Size = System::Drawing::Size(97, 17);
				 this->checkBoxMergeCameras->TabIndex = 17;
				 this->checkBoxMergeCameras->Text = L"MergeCameras";
				 this->toolTip1->SetToolTip(this->checkBoxMergeCameras, L"Check to overwrite cameras from InputM2 with those from InputM2I.");
				 this->checkBoxMergeCameras->UseVisualStyleBackColor = true;
				 // 
				 // checkBoxMergeAttachments
				 // 
				 this->checkBoxMergeAttachments->AutoSize = true;
				 this->checkBoxMergeAttachments->Checked = true;
				 this->checkBoxMergeAttachments->CheckState = System::Windows::Forms::CheckState::Checked;
				 this->checkBoxMergeAttachments->Location = System::Drawing::Point(72, 23);
				 this->checkBoxMergeAttachments->Name = L"checkBoxMergeAttachments";
				 this->checkBoxMergeAttachments->Size = System::Drawing::Size(115, 17);
				 this->checkBoxMergeAttachments->TabIndex = 16;
				 this->checkBoxMergeAttachments->Text = L"MergeAttachments";
				 this->toolTip1->SetToolTip(this->checkBoxMergeAttachments, L"Check to overwrite attachments from InputM2 with those from InputM2I.");
				 this->checkBoxMergeAttachments->UseVisualStyleBackColor = true;
				 // 
				 // checkBoxMergeBones
				 // 
				 this->checkBoxMergeBones->AutoSize = true;
				 this->checkBoxMergeBones->Checked = true;
				 this->checkBoxMergeBones->CheckState = System::Windows::Forms::CheckState::Checked;
				 this->checkBoxMergeBones->Location = System::Drawing::Point(72, 0);
				 this->checkBoxMergeBones->Name = L"checkBoxMergeBones";
				 this->checkBoxMergeBones->Size = System::Drawing::Size(86, 17);
				 this->checkBoxMergeBones->TabIndex = 15;
				 this->checkBoxMergeBones->Text = L"MergeBones";
				 this->toolTip1->SetToolTip(this->checkBoxMergeBones, L"Check to overwrite bones from InputM2 with those from InputM2I.");
				 this->checkBoxMergeBones->UseVisualStyleBackColor = true;
				 // 
				 // buttonGo
				 // 
				 this->buttonGo->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
					 | System::Windows::Forms::AnchorStyles::Right));
				 this->buttonGo->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
					 static_cast<System::Byte>(0)));
				 this->buttonGo->Location = System::Drawing::Point(226, 249);
				 this->buttonGo->Name = L"buttonGo";
				 this->buttonGo->Size = System::Drawing::Size(117, 38);
				 this->buttonGo->TabIndex = 26;
				 this->buttonGo->Text = L"Go!";
				 this->toolTip1->SetToolTip(this->buttonGo, L"Click this to perform operations.");
				 this->buttonGo->UseVisualStyleBackColor = true;
				 this->buttonGo->Click += gcnew System::EventHandler(this, &Form1::buttonGo_Click);
				 // 
				 // label5
				 // 
				 this->label5->AutoSize = true;
				 this->label5->Location = System::Drawing::Point(20, 7);
				 this->label5->Name = L"label5";
				 this->label5->Size = System::Drawing::Size(46, 13);
				 this->label5->TabIndex = 3;
				 this->label5->Text = L"InputM2";
				 this->toolTip1->SetToolTip(this->label5, L"Required. Select an M2 for M2Mod to work with.");
				 // 
				 // buttonInputM2ImpBrowse
				 // 
				 this->buttonInputM2ImpBrowse->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
				 this->buttonInputM2ImpBrowse->Location = System::Drawing::Point(452, 3);
				 this->buttonInputM2ImpBrowse->Name = L"buttonInputM2ImpBrowse";
				 this->buttonInputM2ImpBrowse->Size = System::Drawing::Size(75, 20);
				 this->buttonInputM2ImpBrowse->TabIndex = 5;
				 this->buttonInputM2ImpBrowse->Text = L"...";
				 this->toolTip1->SetToolTip(this->buttonInputM2ImpBrowse, L"Browse...");
				 this->buttonInputM2ImpBrowse->UseVisualStyleBackColor = true;
				 this->buttonInputM2ImpBrowse->Click += gcnew System::EventHandler(this, &Form1::buttonInputM2Browse_Click);
				 // 
				 // tabControl1
				 // 
				 this->tabControl1->Controls->Add(this->tabExport);
				 this->tabControl1->Controls->Add(this->tabImport);
				 this->tabControl1->Location = System::Drawing::Point(12, 12);
				 this->tabControl1->Name = L"tabControl1";
				 this->tabControl1->SelectedIndex = 0;
				 this->tabControl1->Size = System::Drawing::Size(545, 187);
				 this->tabControl1->TabIndex = 25;
				 // 
				 // tabExport
				 // 
				 this->tabExport->Controls->Add(this->panelImputM2Exp);
				 this->tabExport->Controls->Add(this->panelOutputM2I);
				 this->tabExport->Location = System::Drawing::Point(4, 22);
				 this->tabExport->Name = L"tabExport";
				 this->tabExport->Padding = System::Windows::Forms::Padding(3);
				 this->tabExport->Size = System::Drawing::Size(537, 161);
				 this->tabExport->TabIndex = 0;
				 this->tabExport->Text = L"Export";
				 this->tabExport->UseVisualStyleBackColor = true;
				 // 
				 // panelImputM2Exp
				 // 
				 this->panelImputM2Exp->Controls->Add(this->textBoxInputM2Exp);
				 this->panelImputM2Exp->Controls->Add(this->label1);
				 this->panelImputM2Exp->Controls->Add(this->buttonInputM2ExpBrowse);
				 this->panelImputM2Exp->Location = System::Drawing::Point(6, 9);
				 this->panelImputM2Exp->Name = L"panelImputM2Exp";
				 this->panelImputM2Exp->Size = System::Drawing::Size(531, 24);
				 this->panelImputM2Exp->TabIndex = 19;
				 // 
				 // textBoxInputM2Exp
				 // 
				 this->textBoxInputM2Exp->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
					 | System::Windows::Forms::AnchorStyles::Right));
				 this->textBoxInputM2Exp->Location = System::Drawing::Point(72, 3);
				 this->textBoxInputM2Exp->Name = L"textBoxInputM2Exp";
				 this->textBoxInputM2Exp->Size = System::Drawing::Size(374, 20);
				 this->textBoxInputM2Exp->TabIndex = 4;
				 // 
				 // panelOutputM2I
				 // 
				 this->panelOutputM2I->Controls->Add(this->buttonOutputM2IBrowse);
				 this->panelOutputM2I->Controls->Add(this->textBoxOutputM2I);
				 this->panelOutputM2I->Controls->Add(this->label7);
				 this->panelOutputM2I->Location = System::Drawing::Point(5, 38);
				 this->panelOutputM2I->Name = L"panelOutputM2I";
				 this->panelOutputM2I->Size = System::Drawing::Size(532, 26);
				 this->panelOutputM2I->TabIndex = 20;
				 // 
				 // textBoxOutputM2I
				 // 
				 this->textBoxOutputM2I->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
					 | System::Windows::Forms::AnchorStyles::Right));
				 this->textBoxOutputM2I->Location = System::Drawing::Point(73, 3);
				 this->textBoxOutputM2I->Name = L"textBoxOutputM2I";
				 this->textBoxOutputM2I->Size = System::Drawing::Size(374, 20);
				 this->textBoxOutputM2I->TabIndex = 7;
				 // 
				 // tabImport
				 // 
				 this->tabImport->Controls->Add(this->panelInputM2Import);
				 this->tabImport->Controls->Add(this->panelInputM2I);
				 this->tabImport->Controls->Add(this->panelOutputM2);
				 this->tabImport->Controls->Add(this->panelImportCb);
				 this->tabImport->Location = System::Drawing::Point(4, 22);
				 this->tabImport->Name = L"tabImport";
				 this->tabImport->Padding = System::Windows::Forms::Padding(3);
				 this->tabImport->Size = System::Drawing::Size(537, 161);
				 this->tabImport->TabIndex = 1;
				 this->tabImport->Text = L"Import";
				 this->tabImport->UseVisualStyleBackColor = true;
				 // 
				 // panelInputM2Import
				 // 
				 this->panelInputM2Import->Controls->Add(this->textBoxInputM2Imp);
				 this->panelInputM2Import->Controls->Add(this->label5);
				 this->panelInputM2Import->Controls->Add(this->buttonInputM2ImpBrowse);
				 this->panelInputM2Import->Location = System::Drawing::Point(6, 9);
				 this->panelInputM2Import->Name = L"panelInputM2Import";
				 this->panelInputM2Import->Size = System::Drawing::Size(531, 24);
				 this->panelInputM2Import->TabIndex = 26;
				 // 
				 // textBoxInputM2Imp
				 // 
				 this->textBoxInputM2Imp->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
					 | System::Windows::Forms::AnchorStyles::Right));
				 this->textBoxInputM2Imp->Location = System::Drawing::Point(72, 3);
				 this->textBoxInputM2Imp->Name = L"textBoxInputM2Imp";
				 this->textBoxInputM2Imp->Size = System::Drawing::Size(374, 20);
				 this->textBoxInputM2Imp->TabIndex = 4;
				 // 
				 // panelInputM2I
				 // 
				 this->panelInputM2I->Controls->Add(this->label3);
				 this->panelInputM2I->Controls->Add(this->buttonInputM2IBrowse);
				 this->panelInputM2I->Controls->Add(this->textBoxInputM2I);
				 this->panelInputM2I->Location = System::Drawing::Point(6, 36);
				 this->panelInputM2I->Name = L"panelInputM2I";
				 this->panelInputM2I->Size = System::Drawing::Size(531, 30);
				 this->panelInputM2I->TabIndex = 23;
				 // 
				 // textBoxInputM2I
				 // 
				 this->textBoxInputM2I->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
					 | System::Windows::Forms::AnchorStyles::Right));
				 this->textBoxInputM2I->Location = System::Drawing::Point(72, 5);
				 this->textBoxInputM2I->Name = L"textBoxInputM2I";
				 this->textBoxInputM2I->Size = System::Drawing::Size(374, 20);
				 this->textBoxInputM2I->TabIndex = 10;
				 // 
				 // panelOutputM2
				 // 
				 this->panelOutputM2->Controls->Add(this->textBoxOutputM2);
				 this->panelOutputM2->Controls->Add(this->buttonOutputM2Browse);
				 this->panelOutputM2->Controls->Add(this->label2);
				 this->panelOutputM2->Location = System::Drawing::Point(6, 66);
				 this->panelOutputM2->Name = L"panelOutputM2";
				 this->panelOutputM2->Size = System::Drawing::Size(531, 24);
				 this->panelOutputM2->TabIndex = 24;
				 // 
				 // textBoxOutputM2
				 // 
				 this->textBoxOutputM2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
					 | System::Windows::Forms::AnchorStyles::Right));
				 this->textBoxOutputM2->Location = System::Drawing::Point(72, 2);
				 this->textBoxOutputM2->Name = L"textBoxOutputM2";
				 this->textBoxOutputM2->Size = System::Drawing::Size(374, 20);
				 this->textBoxOutputM2->TabIndex = 13;
				 // 
				 // panelImportCb
				 // 
				 this->panelImportCb->Controls->Add(this->checkBoxMergeCameras);
				 this->panelImportCb->Controls->Add(this->checkBoxMergeAttachments);
				 this->panelImportCb->Controls->Add(this->checkBoxMergeBones);
				 this->panelImportCb->Location = System::Drawing::Point(6, 92);
				 this->panelImportCb->Name = L"panelImportCb";
				 this->panelImportCb->Size = System::Drawing::Size(531, 63);
				 this->panelImportCb->TabIndex = 25;
				 // 
				 // label4
				 // 
				 this->label4->AutoSize = true;
				 this->label4->Location = System::Drawing::Point(38, 222);
				 this->label4->Name = L"label4";
				 this->label4->Size = System::Drawing::Size(40, 13);
				 this->label4->TabIndex = 30;
				 this->label4->Text = L"Status:";
				 // 
				 // labelSatus
				 // 
				 this->labelSatus->AutoSize = true;
				 this->labelSatus->Location = System::Drawing::Point(81, 222);
				 this->labelSatus->MinimumSize = System::Drawing::Size(450, 0);
				 this->labelSatus->Name = L"labelSatus";
				 this->labelSatus->Size = System::Drawing::Size(450, 13);
				 this->labelSatus->TabIndex = 29;
				 this->labelSatus->Text = L"Ready...";
				 // 
				 // Form1
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->ClientSize = System::Drawing::Size(569, 309);
				 this->Controls->Add(this->tabControl1);
				 this->Controls->Add(this->label4);
				 this->Controls->Add(this->labelSatus);
				 this->Controls->Add(this->buttonGo);
				 this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
				 this->MinimumSize = System::Drawing::Size(500, 320);
				 this->Name = L"Form1";
				 this->Text = L"M2Mod Redux 4.6.1";
				 this->tabControl1->ResumeLayout(false);
				 this->tabExport->ResumeLayout(false);
				 this->panelImputM2Exp->ResumeLayout(false);
				 this->panelImputM2Exp->PerformLayout();
				 this->panelOutputM2I->ResumeLayout(false);
				 this->panelOutputM2I->PerformLayout();
				 this->tabImport->ResumeLayout(false);
				 this->panelInputM2Import->ResumeLayout(false);
				 this->panelInputM2Import->PerformLayout();
				 this->panelInputM2I->ResumeLayout(false);
				 this->panelInputM2I->PerformLayout();
				 this->panelOutputM2->ResumeLayout(false);
				 this->panelOutputM2->PerformLayout();
				 this->panelImportCb->ResumeLayout(false);
				 this->panelImportCb->PerformLayout();
				 this->ResumeLayout(false);
				 this->PerformLayout();

			 }
#pragma endregion

	private:
		System::Void buttonInputM2Browse_Click(System::Object^  sender, System::EventArgs^  e)
		{
			openFileDialog1->Filter = M2Filter;
			openFileDialog1->FileName = textBoxInputM2Exp->Text;
			openFileDialog1->FileName = textBoxInputM2Imp->Text;

			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				textBoxInputM2Exp->Text = openFileDialog1->FileName;
				textBoxInputM2Imp->Text = openFileDialog1->FileName;
				textBoxOutputM2I->Text = openFileDialog1->FileName + "i";
				textBoxInputM2I->Text = openFileDialog1->FileName + "i";
				textBoxOutputM2->Text = textBoxInputM2Exp->Text->Substring(0, textBoxInputM2Exp->Text->Length - 3) + "2.m2";
			}
		}

	private:
		System::Void buttonOutputM2IBrowse_Click(System::Object^  sender, System::EventArgs^  e)
		{
			saveFileDialog1->Filter = M2IFilter;
			saveFileDialog1->FileName = textBoxOutputM2I->Text;
			if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				textBoxOutputM2I->Text = saveFileDialog1->FileName;
			}
		}

	private:
		System::Void buttonInputM2IBrowse_Click(System::Object^  sender, System::EventArgs^  e)
		{
			openFileDialog1->Filter = M2IFilter;
			openFileDialog1->FileName = textBoxInputM2I->Text;
			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				textBoxInputM2I->Text = openFileDialog1->FileName;
			}
		}

	private:
		System::Void buttonOutputM2Browse_Click(System::Object^  sender, System::EventArgs^  e)
		{
			saveFileDialog1->Filter = M2Filter;
			saveFileDialog1->FileName = textBoxOutputM2->Text;
			if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				textBoxOutputM2->Text = saveFileDialog1->FileName;
			}
		}

	private:
		System::Void buttonGo_Click(System::Object^  sender, System::EventArgs^  e)
		{
			buttonGo->Enabled = false;
			buttonGo->Refresh();
			labelSatus->Text = "Working...";
			labelSatus->Refresh();

			M2Lib::M2* M2 = new M2Lib::M2();

			// Export mode.
			if (tabExport->Visible)
			{
				// Check fields.
				if (textBoxInputM2Exp->Text->Length == 0)
				{
					labelSatus->Text = "Error: No input M2 file Specified.";
					buttonGo->Enabled = true;
					return;
				}

				if (textBoxOutputM2I->Text->Length == 0)
				{
					labelSatus->Text = "Error: No output M2I file Specified.";
					buttonGo->Enabled = true;
					return;
				}

				// import M2
				System::IntPtr StringPointer = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(textBoxInputM2Exp->Text);
				M2Lib::EError Error = M2->Load((Char16*)StringPointer.ToPointer());
				System::Runtime::InteropServices::Marshal::FreeHGlobal(StringPointer);

				if (Error != 0)
				{
					labelSatus->Text = gcnew System::String(M2Lib::GetErrorText(Error));
					buttonGo->Enabled = true;
					return;
				}

				// export M2I
				StringPointer = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(textBoxOutputM2I->Text);
				Error = M2->ExportM2Intermediate((Char16*)StringPointer.ToPointer());
				System::Runtime::InteropServices::Marshal::FreeHGlobal(StringPointer);

				if (Error != 0)
				{
					labelSatus->Text = gcnew System::String(M2Lib::GetErrorText(Error));
					buttonGo->Enabled = true;
					return;
				}

				labelSatus->Text = "Export done.";
			}

			// Import mode.
			if (tabImport->Visible)
			{
				// Check fields.
				if (textBoxInputM2I->Text->Length == 0)
				{
					labelSatus->Text = "Error: No input M2I file Specified.";
					buttonGo->Enabled = true;
					return;
				}

				if (textBoxOutputM2->Text->Length == 0)
				{
					labelSatus->Text = "Error: No output M2 file Specified.";
					buttonGo->Enabled = true;
					return;
				}

				// import M2
				if (textBoxInputM2Imp->Text->Length == 0)
				{
					labelSatus->Text = "Error: No input M2 file Specified.";
					buttonGo->Enabled = true;
					return;
				}

				System::IntPtr StringPointer = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(textBoxInputM2Imp->Text);
				M2Lib::EError Error = M2->Load((Char16*)StringPointer.ToPointer());
				System::Runtime::InteropServices::Marshal::FreeHGlobal(StringPointer);

				if (Error != 0)
				{
					labelSatus->Text = gcnew System::String(M2Lib::GetErrorText(Error));
					buttonGo->Enabled = true;
					return;
				}

				// import M2I
				StringPointer = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(textBoxInputM2I->Text);
				Error = M2->ImportM2Intermediate((Char16*)StringPointer.ToPointer(), !checkBoxMergeBones->Checked, !checkBoxMergeAttachments->Checked, !checkBoxMergeCameras->Checked);
				System::Runtime::InteropServices::Marshal::FreeHGlobal(StringPointer);

				if (Error != 0)
				{
					labelSatus->Text = gcnew System::String(M2Lib::GetErrorText(Error));
					buttonGo->Enabled = true;
					return;
				}

				// export M2
				StringPointer = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(textBoxOutputM2->Text);
				Error = M2->Save((Char16*)StringPointer.ToPointer());
				System::Runtime::InteropServices::Marshal::FreeHGlobal(StringPointer);

				if (Error != 0)
				{
					labelSatus->Text = gcnew System::String(M2Lib::GetErrorText(Error));
					buttonGo->Enabled = true;
					return;
				}

				labelSatus->Text = "Import done.";
			}

			delete M2;

			buttonGo->Enabled = true;
		}
};
}
