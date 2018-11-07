This is the readme document.

This is a test to merget the file using githib

1.	Instructions to install and configure prerequisites or dependencies, if any
--------------------------------------------------------------------------------------------------------------------------------------------------

There are 2 Files needed for the server app. The config.xml and alert_template.html;
	Copy Config.xml and alert_template.html  to the server bin forlder. Also copy POCO bin to server bin forlder
config.xml: This has the client details and there alert triggering conditions. This is used by the server to deside
	   whether an alert has happened and to whome the alert mail has to be send
	Sample:
	
		<clients>
		<client key="DESKTOP-JLRGSHE" mail="ajaiantonykolarikal@yahoo.com">
		  <alert type="memory" limit="10%" />
		  <alert type="cpu" limit="10%" />
		  <alert type="processes" limit="10" />
		</client>

alert_template.html:
		This is template html used to send mail. There are  some  8 parameters that are filled by the server code before sending  the mail.
		They are 
		
		1.{#KEY}		->	PC unique key or hostname
		2.{#MAILD}		->	The mail id assigned for each client pc
		3.{#MEM}		->	Memory usage limit of the client pc from xml
		4.{#CPU}		->	CPU usage limit of client PC from xml.
		5.{#PROCESS}		->	No of process active limit  for the client PC from xml.
		6.{#MEM_USGE}		->	Memory usage of the client 
		7.{#CPU_USGE}		->	CPU usage of client PC
		8.{#PROCESS_USGE} 	->	No of process active for the client PC
		
		The server code fills these value to generate a meaning full alert report

MSSQL server is used
	The user creadentials are 
		username:sa
		password:root
		
	The database name is NBMSCSDB. The table to log data is NBMSCS_log
	Create a MSSQL Data base name  and create the table using the  script.
	
	All messages recieved to the server is written to the table with timestamp;
	
Following third party libraries are used for development
-------------------------------------------------------


All boost and POCO libraries are stored in Source File with ServerApp and Client App
boost (boost_1_60_0)
POCO   (poco-develop)
cpprestsdk

Integration of each library to source code.

Boost:	Boost library is used for advanced string operations	
	The boost libray is downloaded from http://www.boost.org/users/download/#history
	The version choosen was 1.60
	The library us extracted to the parent folder under which the source is place.
	Go to "Developer Command Prompt for VS2015".
	Then go to the boost directory ( cd F:\Work-job\C++ - Chief Software Architect\Arcitect\boost_1_60_0)
	run bootstrap.bat
	After that 
	run .\b2
	This buidling will take some  10 minites;
	At the end of installation the include and lib folder will be displayed(boost_1_60_0 and boost_1_60_0\stage\lib) .
	This is added in projects using relative path.
	
	Example Installation is as follows:
		{
		C:\Program Files (x86)\Microsoft Visual Studio 14.0>cd F:\Work-job\C++ - Chief Software Architect\Arcitect\boost_1_60_0
		
		C:\Program Files (x86)\Microsoft Visual Studio 14.0>f:
		
		F:\Work-job\C++ - Chief Software Architect\Arcitect\boost_1_60_0>bootstrap.bat
		Building Boost.Build engine
		
		Bootstrapping is done. To build, run:
		
		    .\b2
		
		To adjust configuration, edit 'project-config.jam'.
		Further information:
		
		    - Command line help:
		    .\b2 --help
		
		    - Getting started guide:
		    http://boost.org/more/getting_started/windows.html
		
		    - Boost.Build documentation:
		    http://www.boost.org/build/doc/html/index.html
		
		F:\Work-job\C++ - Chief Software Architect\Arcitect\boost_1_60_0>./b2
		
		The Boost C++ Libraries were successfully built!
		
		The following directory should be added to compiler include paths:
		
		    F:\Work-job\C++ - Chief Software Architect\Arcitect\boost_1_60_0
		
		The following directory should be added to linker library paths:
		
		    F:\Work-job\C++ - Chief Software Architect\Arcitect\boost_1_60_0\stage\lib

	}
	
POCO:	POCO library is used to send mail. 	
	The POCO libray is download from https://github.com/pocoproject/poco to parent folder as poco-develop.
	Go to "Developer Command Prompt for VS2015".
	execute build_vs140.cmd from command line. This will take few minutes
	
	The bin folder is created where all the nneded supporting things are created. 
	
	EITHER THE PATH HAS TO BE  ADDED TO SYSTEM PATH OR COPY THESE FILE TO THE EXECUTABLE PATH. LIKE DEBUG OR RELEASE.
	
	The include path is addded in server project as relative path 
	Example Installation is as follows
		{
		C:\Program Files (x86)\Microsoft Visual Studio 14.0>cd F:\Work-job\C++ - Chief Software Architect\Arcitect\poco-develop
		C:\Program Files (x86)\Microsoft Visual Studio 14.0>f:
		F:\Work-job\C++ - Chief Software Architect\Arcitect\poco-develop>build_vs140.cmd
		

		}

cpprestsdk(casablanca) : This is used for REST interface in server and client app. It is added from Nuget packages.
			When the project is loaded if it is not there it will be downloaded
			
			
2.	Instructions to create and initialize the database (if required)	
--------------------------------------------------------------------------------------------------------------------------------------------------
			
	MSSQL  user creadentials are 
			username:sa
			password:root
	
	
	USE [NBMSCSDB]
	GO
	
	/****** Object:  Table [dbo].[NBMSCS_log]    Script Date: 26-09-2016 13:43:57 ******/
	SET ANSI_NULLS ON
	GO
	
	SET QUOTED_IDENTIFIER ON
	GO
	
	CREATE TABLE [dbo].[NBMSCS_log](
		[id] [bigint] IDENTITY(1,1) NOT NULL,
		[client_key] [nvarchar](max) NULL,
		[ram] [float] NULL,
		[cpu] [float] NULL,
		[process] [float] NULL,
		[created_dt] [datetime] NULL,
	 CONSTRAINT [PK_NBMSCS_log] PRIMARY KEY CLUSTERED 
	(
		[id] ASC
	)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
	) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
	
	GO
	
	ALTER TABLE [dbo].[NBMSCS_log] ADD  CONSTRAINT [DF_NBMSCS_log_ram]  DEFAULT ((0)) FOR [ram]
	GO
	
	ALTER TABLE [dbo].[NBMSCS_log] ADD  CONSTRAINT [DF_NBMSCS_log_cpu]  DEFAULT ((0)) FOR [cpu]
	GO
	
	ALTER TABLE [dbo].[NBMSCS_log] ADD  CONSTRAINT [DF_NBMSCS_log_process]  DEFAULT ((0)) FOR [process]
	GO
	
	ALTER TABLE [dbo].[NBMSCS_log] ADD  CONSTRAINT [DF_NBMSCS_log_created_dt]  DEFAULT (getdate()) FOR [created_dt]
	GO


3.	Assumptions you have made - it is good to explain your thought process and the assumptions you have made
--------------------------------------------------------------------------------------------------------------------------------------------------
Run both app as administrator. Then only you can use the IP, other wise has to use localhost.

Server App:
	Each Client stations hostname and limit parameters are to be entered in the config.xml. When ever config.xml is update it is reloaded for every 
	request. SO there is no need to restart the server

Client App: Once a server IP is specified it cannot be changed without restarting the app.


Running the App: Please disable Antivirus. Avast block sending mail

4.	Requirements that you have not covered in your submission, if any
--------------------------------------------------------------------------------------------------------------------------------------------------
As of my knowledge it is Nil.

5.	Instructions to configure and prepare the source code to build and run properly
--------------------------------------------------------------------------------------------------------------------------------------------------
Install prerequisites inside the parent forder where ServerApp and Client App resides.

	boost (boost_1_60_0)
	POCO   (poco-develop)
	cpprestsdk

All source project uses relative path. There is no hard coding of library path used

6.	Issues you have faced while completing the assignment, if any
--------------------------------------------------------------------------------------------------------------------------------------------------
Nil

7.	Constructive feedback for improving the assignment
--------------------------------------------------------------------------------------------------------------------------------------------------
If you provide a desigh document template it will be east to follow a standard.

--------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------Project Structure Source Folder-------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------

ServerApp:
	ServerApp -  Entry point; Main APp. Invokes the http listener and process message using Serverlib.
	Serverlib-	The lib having the functionality
	ServerUnitTest:	The Unit testing code
ClientApp:
	ClientApp: Sends the Rest request and waits for 5 minitues, in a infinite look. Calls the client lib to get system parameters.
	Clientlib: Gets system paramters.
	ClientlibUnitTest: Unit test for client lib functions.
boost_1_60_0
poco-develop
config.xml
alert_template.html
	
-------------------------------------------------------------------------------------------------------------------------------------------------


			
			
