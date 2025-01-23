<?php

define("ACG_VERSION","2.2");

/*

<pre>
      This class creates source code for ATK nodes based on their database definitions
      
      @Author Jorge Garifuna (GariDigital.com) <info@GariDigital.com>
      
      Version: 2.2
      Originally Created: May 1, 2006      

      Updated: 
      January 19, 2007
      - added ability to change menu label directly from configuration view, prior to code generation
      - added global navigation for all node with "Sliding Doors Tabs Menu" from http://www.dynamicdrive.com/style/csslibrary/item/sliding-doors-tabs-menu/
          * a new node is created with the name <moduleName>_menu, which is included to the following actions of each node, if the node is included on the menu
               *    view, add, edit, admin
          * option automatically selected if more than 2 nodes exist for creation
          * This requires a module called "gktools" to exist, along with with the subfiles:
          *    css/sliding_doors_tab_chrome.css
          *    js/pngfix.js
          * Image code is generated to allow the developer to add icons next to the menus, but commented out, since images names cannot be determined at generation time                                                                        

      - added table list overflow to generator database selection view
      - added table node overflow to generator creation view
      
      January 16, 2007
      - added tab and menu language support
      
      January 5, 2007
      - added language support for: English  Spanish  Dutch  German  
      
      December 30, 2006
      - added support for atkDurationAttribute
      - added support for atkTimeAttribute
      - added support for atkCountryAttribute
      - added support for atkStateAttribute (required ATK 5.7)
      - changed default option of enum db type from radio to list
      - added support for AF_LARGE
      - enabled flag AF_ONETOMANY_EMBED and added validate() and getMyNextId() functions
      
      December 22, 2006
      - added drop down list to choose number of records per page on admin mode
      - added number option to fields of type decimal in addition to currency
      - added $size=10, $decimals=0 values to atkNumberAttribute
      
      December 19, 2006
      - added updated date and version 1.0 to aminFooter()
      
      December 14, 2006
      - changed default FCK dimension to 450/200
      December 6, 2006
      - added FCK toolbar option selection between Default and Basic, defaulting to Basic toolbar
      - Change default type of text fields to atkFCKAttribute from atkTextAttribute
      
      November 16, 2006
      - fixed getMenuCode() to include permission of actual toggle field name instead of hardcoded action "toggle"
      - added AF_POPUP flag and automatically select for attributes containing "url" in the name
      - added support for atkUrlAttribute and automatically select for attributes containing "url" in the name
      - added display() function of descriptor attribute to link to either view or edit mode
      - added trackRecord() to Obtain date, and user info to track record changes.
               Assumes existance of attributes: :
                    - created_date {date or datetime}, created_by {int or varchar}, created_by_id {int}
                    - modified_date {date or datetime}, modified_by {int or varchar}, modified_by_id {int}
     - update preAdd() and preUpdate() to call new function trackRecord() in case fields exist to track record
                    
      November 7, 2006
      - added display function to fck attributes
      
      October 21, 2006
      - added code to  adminHeader() to display messages on the queue on list view, if present 
      - added preCopy() to unset primary field, on editcopy, to avoid SQL duplicate error
      - added function getSideNodesId() and call from preAdd(), to get last ID inserted to database on intermediary nodes.       This is do to the breakaway of the dependency of db_sequence
      - display checkbox to allow option to indicate intermediary node on interface, if node has more than 1 m2o relation
          * this makes shuffle and manybool relations work out of the box, without depending on db_sequence
          * when the checkbox is selected, code is added to automatically pull the latest primary values of the side nodes 

      - added function getMyLastId() and call from postAdd(), to get last ID inserted to database. This is do to the breakaway of the dependency of db_sequence

      
      September 30, 2006
      - added event log
          if dbsequence is not being used, then you may want to make the following modifications to the ATK file:
          atk/utils/class.atkeventlog.inc

          as follows:
                    ... $primarykey = $db->escapeSQL($this->m_node->primaryKey($record));
      
                    // added by Jorge Garifuna (info@GariDigital.com) on 9/30/06
                    // probably dbsequence is not being used and valu of primary key is null
                    // this it how we attempt to get the value of the latest primary field of the table for the calling node
                    // disclaimer: use this at your own risk. I'm not sure how accurate this will be in a high traffic system
                     if(strstr($primarykey, "NULL") && $action=="save"){ // eg: bcm_contact.bcm_contact_id='NULL'
                         //$module = $this->m_node->m_module;
                         $table = $this->m_node->m_table; // grab table for node
                         list($fpk, $val) = explode("=",$primarykey); // eg: bcm_contact.bcm_contact_id='NULL'
                         if(!empty($fpk)){ // onnly proceed if primary key section available
                              list($n, $mpk) = explode(".",$fpk); // eg: bcm_contact.bcm_contact_id
                              if(!empty($mpk) && !empty($table)){ // only proceed if primary key exists and table exists
                                   // grab the maximum primary key value available on calling node
                                   // this will probably fail if primary key is not numerical in value
                                   $query = "SELECT max($mpk) as pkval FROM $table"; 
                                   $rows = $db->getrows($query);
                                   if(count($rows)>0){
                                        $pkval = $rows[0]["pkval"];
                                        $primarykey = $fpk."='$pkval'";  
                                        $primarykey = $db->escapeSQL($primarykey);                                                
                                   }
                                   //die($query."<br>".$primarykey);
                              }// end if
                         }           
                     }// end if
                     // end Jorge Garifuna addition of 9/30/06
          
                //$db->query("INSERT INTO atkeventlog (id, userid, stamp, node, action, primarykey)
                //              VALUES(".$db->nextid("atkeventlog").", $userid, ".$db->func_now().", '$node', '$action', '$primarykey')");
          
                $db->query("INSERT INTO atkeventlog (userid, stamp, node, action, primarykey)
                              VALUES($userid, ".$db->func_now().", '$node', '$action', '$primarykey')");
      
      August 20, 2006
      - added width and height to FCK attribute
      
      August 2, 2006
      - removed use of AF_AUTOKEY in favor of AF_PRIMARY|AF_HIDE to avoid problems with db_sequence table
      - added action_save function to remove AF_PRIMARY flag prior to record saving to avoid problems with db_sequence table
               * this was a suggestion of gabilan on http://achievo.org/forum/viewtopic.php?t=1620
      
      July 31, 2006
      - fixed huge spacing added to config.modules.inc
      
      July 30, 2006
      - added index field as part of messaging notifications to; postUpdate, postAdd and postDelete
      - added toggle field to registerNode section of module.inc so it registers on users security profiles
      - added the following specific flags to atkFileAttribute :
          AF_FILE_NO_UPLOAD 
          AF_FILE_NO_SELECT 
          AF_FILE_NO_DELETE 
          AF_FILE_PHYSICAL_DELETE 
          AF_FILE_NO_AUTOPREVIEW 
          AF_FILE_POPUP
          
      July 23, 2006
      - added global search option
      - enabled descriptor option by default to accomodate for global search results
      - Please read "How to add Global Search to ATK applications" from http://www.achievo.org/wiki/Howto%27s to learn to enable global search on ATK
      
      
      July 11, 2006
      - made user_id field AF_HIDE by default
      
	 July 3, 2006
      - change node parent contructor call from "atkNode("node_name"..." to "atkNode($this->node," as recommended by Fabio Saitta  at http://achievo.org/forum/viewtopic.php?t=1425
      
      June 28, 2006
      - fixed: removed print line, which was causing warnings on *unix systems from normalize_relations() function
      
      June 27, 2006
      - added flag AF_HIDE
	 - added messaging option, atk.utils.atkmessagequeue, to each node to notify the user when a record has been added, updated or deleted
      - added security access to main module menu so that users wont see the module menu, if they don't have permission to any of the nodes of the menu 
      - added security access to nodes menu, so that users won't see those nodes they do not have access to
       
      June 26, 2006
      - removed space from bottom of node names that caused php errors on certain systems
      - added relation to view children of those nodes containing parent_id field
      - included call to function atkversion() to automatically obtain current ATK version
      - added contant  ACG_VERSION and display generator version at the bottom of the screen
      
	 June 20, 2006
	 - removed new line (\n) from begining of generated nodes
	 
	 June 18, 2006
	 - added atkMultiSelectAttribute support for DB type "set" 
	 	* added function setDelimeter	to atkMultiSelectAttribute. If this has not been added to the ATK core, you may have to get the updated code from:
	 	* http://achievo.org/forum/viewtopic.php?p=6027	 	

	 - added atkListAttribute support as an alternative to Radio buttons for DB Type "enum" 
	 
      June 16, 2006
      - give user ability to choose the folder destination for atkFileAttribute
      - replaced spaces from module name with underscore (_)
      - added variable $atk_version so we make distinctions between some items that are supported under atk 5.6
      - added atkRowCounter for row counter if $atk_version >=5.6  
      
	 June 14, 2006
	 - added smart search
      - added header/footer note fields for user to add notes to show up on the generated nodes
      - added drop down list to allow user to select ATK attribute types for varchar and text types
	 - added FCK to attribute options 
	 
      June 13, 2006
      - added option to include or exclude node from ATK menu
      - added atkShuffleRelation
      - added atkManyToBoolRelation
      - added getNodes() function to module.inc of the code generator as per recomendation of from Fabio Saitta  http://achievo.org/forum/viewtopic.php?t=1224&postdays=0&postorder=asc&start=15
      - color coded node name on option wizard window to easily spot
      - reduced font size of header labels on option window to attempt to fit all options on screen
      - replaced spaces on header labels with <br> to attempt to fit options on screen
      - replaced underscores (_) on relationships display name with break (<br>) to attempt to fit options on screen
      
      - added atkEmailAttribute for fields with name 'email'
      - added atkPasswordAttribute for fields with name 'password'
      - added function recordActions()  for edit after copy
      - added node flags: 
               2.7 NF_IMPORT 
               2.8 NF_EXPORT 
               2.9 NF_NO_ADD 
               2.10 NF_NO_DELETE 
               2.11 NF_NO_EDIT 
               2.12 NF_NO_VIEW 
               2.13 NF_NO_SEARCH 
               2.14 NF_NO_EXTENDED_SEARCH 
               2.15 NF_NO_FILTER 
               2.16 NF_NO_SECURITY 
               2.17 NF_LOCK 

      June 12, 2006
      - added atkManyToOneTreeRelation to those nodes that contain a field name parent_id or manager_id
      - added rowColor and changed default colors
      
      June 8, 2006
      - fixed attributes folder creation in the absense of the file folder
      - fixed toggle field color order
      - added visualization colors on form items click on node detail options form rows
      
	 June 7, 2006
	 - included 'row counter' option to each node
	 - made dispatch url relative instead of absolute to eliminate one extra step from installation
	 - allow user to change attribute position. o2m relations are now shown at the end of the node
	 - implemented toggle color field so that if a toggle field is active, then the specified color on is applied during display 
					these colors can be changed at the following variable
					$function_includes_array[action_toggle][field][active][checked_color]
					$function_includes_array[action_toggle][field][active][unchecked_color]
	 
	 June 6, 2006
	 - localized attributes folder to use counterattribute
	 - included multiple toggle fields and their checked and unchecked colors
 
	 June 5, 2006
	 - fixed active link 
	 
	 June 4, 2006 



	 Disclaimer: Use this code at your own risk. The author has done his best to eliminate as many bugs as possible
		It is highly recommended that you backup all your files prior to running this script.
		The author is not reponsible for any damages resulting from the use of this script.
		
	Having said that....
	
	Installation:
		  You can install this script like you would any other ATK script. Here is what I recommend:
		  
		  1. create a folder called 'gkcode_generator'  within the 'modules' folder of ATK
		  2. Copy this script to the folder
		  3. modify your config.module.inc to include the following line: module('gkcode_generator');
		  4. the module.inc file withing the 'gkcode_generator' folder should look as follows:

					
					  class mod_gkcode_generator extends atkModule
					  {           
					    function getMenuItems()
					    {          
					          $menu_label = 'ATK Code Generator';
					          $module = 'gkcode_generator';
					          // parent menu
							$this->menuitem($menu_label); 		
								$this->menuitem('Code Generator<hr>', dispatch_url($module.'.code_generator', 'intro'), $menu_label); 	
					    }// end mod
					    
					  }
					

		5. modify the variable '$destination_folder'' below to the folder where you want the generated code to go
		6. fire up ATK and click on the menu link 'ATK Code Generator' from the left
		7. place a check mark next to the tables you want ATK code to be generated for and click on the 'next' button at the bottom of the page
		8. enter a module name at the top of the page and select/deselect the options you want for each node. For the most part the default options will work fine
		9. click on the 'Generate Code' button located at the bottom of the page
		10. If you selected to have your new module created within the 'modules' folder of ATK, 
				then you can test your new module by simply clicking on the menu link 'Back to Main' 
				and you will see the name of the new module within the menu left menu of ATK 
		11. If you selected for the generated code to be stored on a different location 
				then go to that location to see the generated files and copy them to the appropriape ATK path to see them in action 
		  

</pre>		  

*/

/*

to-do notes (1/5/06)
- add language for menues and tabs (done 1/16/07)

to-do notes (12/30/06)
- atkDurationAttribute (done)
- atkTimeAttribute (done)
- atkCountryAttribute (done 12/30/06)
- atkStateAttribute (done 12/30/06)


to-do notes (6/18/06)
- show list and checkbox options next to radio types on wizard


to-do notes (6/16/06)
- add AF_ONETOMANY_EMBED (done 12/30/06)
- add the following specific flags to atkFileAttribute (done 7/30/06):
     AF_FILE_NO_UPLOAD 
     AF_FILE_NO_SELECT 
     AF_FILE_NO_DELETE 
     AF_FILE_PHYSICAL_DELETE 
     AF_FILE_NO_AUTOPREVIEW 
     AF_FILE_POPUP
     
to-do notes (6/14/06)
- o2o relation
- specific flags for
     * atkDateAttribute
     * atkFileAttribute (done 7/30/06)
     * atkPasswordAttribute
     * atkRadioAttribute
     * atkManyToOneRelation
     * atkOneToOneRelation
     * atkOneToManyRelation
- atkFileAttribute folder option (done)

to-do notes (6/12/06)
 - add smart search option (done 6/14/06)

to-do notes (6/7/06)
 - append security option to each menu item so that it dissappears from menu if user is not assigned to it
 - add counter node to all generated nodes so that the record# is displayed on the first column (done 6/8/06)
 
to-do notes (6/3/06)
 - implement relations for atkshuttlerelation (done 6/13/06)
 - implement relations for atkmanyboolrelation (done 6/13/06)
 - add option to include or exclude node from menu (done 6/13/06)
 - either allow copy or edit after copy, but not both. Make group radio
 - create menu file, module.inc (done 6/4/06)
 - create module file, config.modules.inc  (done 6/4/06)
 - check for backup flag before generating node files  (done 6/4/06)
 - eventually scan existing node files and copy/merge all new functions to new node file
 - create modules to be plug and play right out of the box  (done 6/4/06)
 - create securities options for each node: "admin", "add", "edit", "delete", "grantall" 

to-do notes (5/27/06)
 - automatically check box (AF_RELATION_AUTOLINK) with m2o relation field (done 5/27/06)
 - add descriptor radio and automatically select first varchar field if node contains o2m relation (done 5/28/06)
 - create relation code for: (done 5/27/06)
 	* meta nodes 
 	* tree nodes 	
 - create helper functions:
 	* preAdd (done 5/31/06)
 	* preUpdate (done 5/31/06)
 	* initial_values (done 5/31/06)
 	* action_toggle (done 5/31/06)
 	* active_display (done 5/31/06)
 	* adminHeader (done 5/31/06)
 	* adminFooter (done 5/31/06)
 	* descriptor (done 5/31/06)
 	* editPage (done 6/3/06)
 	* viewPage (done 6/3/06)
 - create node code with: (done 5/28/06)
 	* node type
 	* node name
 	* node variables
 	* node constructor
 	* node flags
 	* already created attributes 
 	* already created relations
 
 - implement code for edit after copy	 	 	 	 	 	  	

to-do notes (5/20/06)
 - fix relations bug (done 5/25/06)
 - include enum values for input to ATK Radio (done 5/27/06)
 - write JS to check for module name (done 5/26/06)
 - implement tree node code
 - check for backup flag
 - write JS to enable all disabled fields (exclude text fields, only include checkboxes) prior to submission to obtain:
 	* primary key
 	* obligatory
 	*  (done 5/26/06) 	
 
to-do notes (5/2/06)
 - make first field of type varchar searchable  (done 5/2/06)
 - make fields with names image, photo,file, attachment of file type (done 5/2/06)
 - make created_date hide add, hide list, read only (done 5/2/06)
 - make modified_date hide add, hide list, read only (done 5/2/06)
 - add check box to create either regular node or meta node next to module name (done 5/2/06)
 - add check box to back up existing files next to module name
 - add attribute tab field (done 5/2/06)
 - hide "text" type fields from list  (done 5/2/06)
 - only allow one field for index  (done 5/2/06)
 - include relationships with option to create code or not. Relations should automatically be detected, if the primary key of a table is located as a non-primary key on other table (done 5/2/06)
  
*/


  //atkimport("atk.atkmetanode");
  useattrib("atkboolattribute");
  useattrib("atkradioattribute");
  useattrib("atknumberattribute");
  useattrib("atkcurrencyattribute");
  useattrib("atkdateattribute");
  userelation("atkonetomanyrelation");  
  userelation("atkmanytoonerelation");  
  useattrib("gkcode_generator.counterattribute");  
  //useattrib("atkcalculatorattribute");
  useattrib("atkwysiwygattribute"); // added 6/14/06
  


  class code_generator extends atkNode
  {
          var $atk_version;// = 5.6;
          // var $dispatch_url 		= "/gktools/atk/gn/dispatch.php"; // change this to fit your needs
          var $dispatch_url 			= "./index.php"; // change this to fit your needs
		/* 
			destination of modules generated by this code (relative to code generator path)
			the last slash (/) is necessary
			the default "../../modules/" is the current modules folder of ATK
			if for example you use "modules/" a folder called "modules" will be created within the folder of the code generator script
		
		*/
		var $destination_folder 		= "../../modules/";  

		var $module 				= "gkcode_generator";     // change this to fit your needs
          var $node 				= "code_generator";           // change this to fit your needs

		var $attachment_files_folder 	= "files"; // name of folder for file attachments, this could be overwritten through user interface
		var $attachment_files_detected= false; 	// flag to know if a node contains fields that indicate addition of files. This is the default value, so do not change
          var $default_attachment_files_folder;   // dont change this. it gets initialized to 	$attachment_files_folder in the constructor	
          var $attachment_files_folder_array = array();     // to store all possible user selected file folder names for physical creation
          var $row_counter_detected 	= false;		// flag to detect if the attribute folder should be created for the row counter attribute
		var $backup_folder			= "backup"; // name of backup folder
		var $module_config_file 		= "../config.modules.inc"; // the path to the file config.modules.inc (relative to code generator path) 
		var $module_menu_file 		= "module.inc"; // the name of the file where the menu configuration goes
		var $row_counter_file 		= "attributes/class.counterattribute.inc"; // the path to the file class.counterattribute.inc (relative to code generator path)
          var $header_font_size         = "xx-small"; // header font size for css style tag
          var $enable_node_notes         = true;   // you can change this to true or false to display the fields to add header/footer notes
          var $form_name 			= "table_names";
          var $form_action;
          var $db_name;
		var $node_flags_array;
		var $attribute_flags_array;
		var $file_attribute_flags_array;  // added 7/30/06
		var $relations_array;
		var $relation_type_array;
		//var $colors_array 		= array("blue");
		var $date_format 			= "m/d/y";
		var $preadd_field 			= "created_date"; // field included on the preAdd function, if exists on table

		var $user_field 			= "user_id";
		var $selected_node_records_row; // added 12/22/06 to keep track of the number of records per row of a given node
          var $parent_fields_array      = array("state"=>false,
                                                "field"=>array(
                                                            "parent_id"=>
                                                                      array(
                                                                           "state"=>false,
                                                                           "flags"=>"AF_PARENT"
                                                                      ),
                                                            "manager_id"=>
                                                                      array(
                                                                           "state"=>false,
                                                                           "flags"=>"AF_PARENT"
                                                                      )                                                                      
                                                         )
                                             ); // for tree relationship
          
		// functions to include
		var $function_includes_array 	= array(
									"getMessage" => array(
											"state"=>false),
                                             "recordActions" => array(
											"state"=>false,
											"field"=>"NF_EDITAFTERCOPY"),
                                             "preAdd" => array(
											"state"=>false,
											"field"=>"created_date"),
                                             "preAddIntermediary" => array(
											"state"=>false,
                                                       "values"=>""),
								 	"preUpdate"=>array(
											"state"=>false,
											"field"=>"modified_date"),
								 	"descriptorLink"=>array(
											"state"=>false,
											"field"=>"", // dynamic based on field
                                                       "mode"=>""), // mode: view, edit
								 	"initial_values"=>array(
											"state"=>false,
											"field"=>"active"),
								 	"action_toggle"=>array(
											"state"=>false,
											"field"=>array(
                                                                 "active"=>array(
                                                                           "state"=>false,
                                                                           "checked_color"=>"",
                                                                           "unchecked_color"=>"#FFFBD0"
                                                                 ),
                                                                 "enabled"=>array(
                                                                           "state"=>false,
                                                                           "checked_color"=>"",
                                                                           "unchecked_color"=>"#FFFBD0"
                                                                  ), 
                                                                 "approved"=>array(
                                                                           "state"=>false,
                                                                           "checked_color"=>"#DEE79C", //99CC99
                                                                           "unchecked_color"=>"#BBD9EE"
                                                                 ),
                                                           )
                                             ),
/*								 	"active_display"=>array(
											"state"=>false,
											"field"=>"active"),
								 	"enabled_display"=>array(
											"state"=>false,
											"field"=>"enabled"),
*/
								 	"adminHeader" => true,
								 	"adminFooter"=>true,
								 	"descriptor"=>true, // enabled 7/23/06s
									"viewPage"=>true, 	 	 	 	 	 	  
									"editPage"=>true

					);
		
		// attributes name reference
		var $attributes = array(
							"attribute" =>"atkAttribute",
							"number" =>"atkNumberAttribute",
							"currency" =>"atkCurrencyAttribute",
							"bool" =>"atkBoolAttribute",
							"radio" =>"atkRadioAttribute",
							"file" =>"atkFileAttribute",
							"color" =>"atkColorPickerAttribute", 
							"date" =>"atkDateAttribute",
							"datetime" =>"atkDateTimeAttribute",
							"text" =>"atkTextAttribute",
							"email" => "atkEmailAttribute",
							"password" => "atkPasswordAttribute",
							"fck" => "atkFckAttribute",
							//"mce" => "MceAttribute",
							"multilist" =>"atkMultiSelectAttribute",
							"list" =>"atkListAttribute",
                                   "url" => "atkUrlAttribute", // added 11/16/06
                                   "country"=>"atkCountryAttribute", // added 12/30/06
                                   "state"=>"atkStateAttribute",	// added 12/30/06
                                   "duration"=>"atkDurationAttribute",	// added 12/30/06
                                   "time"=>"atkTimeAttribute",	// added 12/30/06						
						);
		var $mce_module = "gktools";	// the module where MCE is located
		var $related_attributes_array = array();
		var $fck_attributes_array = array(); // holder for FCK attributes to add display() to each one, added 11/7/06

          var $fck_attribute_options_array = array("toolbar"=>array("Default","Basic"));  // added 12/6/06
          var $fck_toolbar_type; // holds the type of FCK toolbar
          var $add_validate; // 
          var $supported_languages = array(
                                        "en"=>array("label"=>"English","selected"=>true),
                                        "es"=>array("label"=>"Spanish","selected"=>true),
                                        "dl"=>array("label"=>"Dutch","selected"=>false),
                                        "de"=>array("label"=>"German","selected"=>false),
                                   );
		var $global_navigation = false; // added 1/19/07, signal to add global navigation menu

		  function __construct()
		  {
			$this->code_generator();
		  }
          function code_generator(){

               $this->atkNode("code_generator",NF_ADD_LINK | NF_MRA);               
               $this->form_action = array("next"=>"Next",
                                   "finish"=>"Generate_Code");                                 

               $dispatch_params = "?atknodetype=".$this->module.".".$this->node."&atklevel=1&atkprevlevel=0&atkstackid=445670af19b1b&";
               $this->dispatch_url .= $dispatch_params;
               global $config_db;
               $this->db_name = $config_db["default"]["db"];                  
               
               $this->relation_type_array = array("o2m"=>"o2m","m2o"=>"m2o","tm2o"=>"tree m2o","shuffle"=>"shuffle","bool"=>"manybool");
               $this->relations_array = array();
               
               // initialize default file folder
               $this->default_attachment_files_folder = $this->attachment_files_folder;
               
               $this->load_node_flags();
               $this->load_attribute_flags();
               $this->load_file_attribute_flags();
               $this->load_related_attributes();

               $this->atk_version = atkversion();

		}
		
		function load_related_attributes(){
			// start loading related attributes
			$this->related_attributes_array = array(
											"varchar" => array(					// varchar
														$this->attributes["email"],
														$this->attributes["password"],
														$this->attributes["attribute"],
														$this->attributes["file"],
														$this->attributes["color"],
                                                                      $this->attributes["url"],
                                                                      $this->attributes["country"],
                                                                      $this->attributes["state"],											
                                                                      $this->attributes["duration"],											
													),
											"text" => array(					// text
														$this->attributes["text"],
														$this->attributes["fck"],
														//$this->attributes["mce"],
														$this->attributes["attribute"]			
													),
											"enum" => array(					// enum
														$this->attributes["radio"],
														$this->attributes["list"],		
													),
											"set" => array(					// set
														$this->attributes["multilist"],
														$this->attributes["radio"],
														$this->attributes["list"],		
													),
											"decimal" => array(					// decimal
														$this->attributes["number"],
														$this->attributes["currency"]		
													)

									);
		}// end function
		
		/*
			loads node flags for wizard
		*/
		function load_node_flags(){
			$this->node_flags_array = array();
									// node_flags($flag_name, $label, $type="checkbox", $default_value=1){
			$this->node_flags_array[] = new node_flags("NF_ADD_LINK", "Add Link", "checkbox", 1);					
			$this->node_flags_array[] = new node_flags("NF_MRA", "Multi Record Action", "checkbox", 1);
			$this->node_flags_array[] = new node_flags("NF_TRACK_CHANGES", "Track Changes", "checkbox", 1);					
			$this->node_flags_array[] = new node_flags("NF_ADDAFTERADD", "Add after add", "checkbox", 1);					
			$this->node_flags_array[] = new node_flags("NF_EDITAFTERADD", "Edit after add", "checkbox", 0);					
			$this->node_flags_array[] = new node_flags("NF_COPY", "Copy", "checkbox", 0);					
			$this->node_flags_array[] = new node_flags("NF_EDITAFTERCOPY", "Edit after copy", "checkbox", 0);									
               $this->node_flags_array[] = new node_flags("NF_IMPORT", "Import", "checkbox", 0);
			$this->node_flags_array[] = new node_flags("NF_EXPORT", "Export", "checkbox", 0);
			$this->node_flags_array[] = new node_flags("NF_NO_ADD", "No Add", "checkbox", 0);
			$this->node_flags_array[] = new node_flags("NF_NO_DELETE", "No Delete", "checkbox", 0);
			$this->node_flags_array[] = new node_flags("NF_NO_EDIT", "No Edit", "checkbox", 0);
			$this->node_flags_array[] = new node_flags("NNF_NO_VIEW", "No View", "checkbox", 0);
			$this->node_flags_array[] = new node_flags("NF_NO_SEARCH", "No Search", "checkbox", 0);
			$this->node_flags_array[] = new node_flags("NF_NO_EXTENDED_SEARCH", "No Extended Search", "checkbox", 0);
			$this->node_flags_array[] = new node_flags("NF_LOCK", "Lock", "checkbox", 0);
			$this->node_flags_array[] = new node_flags("NF_CACHE_RECORDLIST", "Cache Record List", "checkbox", 0);
			$this->node_flags_array[] = new node_flags("NF_NO_SECURITY", "No Security", "checkbox", 0);			

		}// end function


		/*
			loads attribute flags for wizard
		*/
		function load_attribute_flags(){
			$this->attribute_flags_array = array();
									// attribute_flags($flag_name, $label, $type="checkbox", $default_value=1){
			//$this->attribute_flags_array[] = new attribute_flags("AF_AUTOKEY", "Primary Key", "checkbox", 1);
               $this->attribute_flags_array[] = new attribute_flags("AF_PRIMARY|AF_HIDE", "Primary Key", "checkbox", 1);	
               				
			$this->attribute_flags_array[] = new attribute_flags("AF_OBLIGATORY", "Obligatory", "checkbox", 0);					
			$this->attribute_flags_array[] = new attribute_flags("AF_HIDE_LIST", "Hide List", "checkbox", 0);					
			$this->attribute_flags_array[] = new attribute_flags("AF_HIDE_ADD", "Hide Add", "checkbox", 0);					
			$this->attribute_flags_array[] = new attribute_flags("AF_HIDE", "Hide", "checkbox", 0);
			$this->attribute_flags_array[] = new attribute_flags("AF_READONLY", "Read Only", "checkbox", 0);					
			$this->attribute_flags_array[] = new attribute_flags("AF_SEARCHABLE", "Searchable", "checkbox", 0);					
			$this->attribute_flags_array[] = new attribute_flags("AF_TOTAL", "Total", "checkbox", 0);					
			$this->attribute_flags_array[] = new attribute_flags("__index__", "Index", "radio", 0);					
			$this->attribute_flags_array[] = new attribute_flags("__descriptor__", "Descriptor", "radio", 0);
			$this->attribute_flags_array[] = new attribute_flags("__order__", "Order", "checkbox", 0);					
			$this->attribute_flags_array[] = new attribute_flags("__forceupdate__", "Force Update", "checkbox", 0);
			$this->attribute_flags_array[] = new attribute_flags("AF_POPUP", "Popup", "checkbox", 0);
			$this->attribute_flags_array[] = new attribute_flags("AF_RELATION_AUTOLINK", "m-to-1 Auto Link Relation", "checkbox", 0);
     		$this->attribute_flags_array[] = new attribute_flags("AF_LARGE", "m-to-1 Large Items", "checkbox", 0);
			$this->attribute_flags_array[] = new attribute_flags("AF_ONETOMANY_EMBED", "1-to-n Embed Relation", "checkbox", 0); // enabled 12/30/06



		}// end function

          // loads flags speific to atkFileAttribute
		function load_file_attribute_flags(){
			$this->file_attribute_flags_array = array();
									// attribute_flags($flag_name, $label, $type="checkbox", $default_value=1){
			$this->file_attribute_flags_array[] = new attribute_flags("AF_FILE_NO_UPLOAD", "No Upload", "checkbox", 0);					
			$this->file_attribute_flags_array[] = new attribute_flags("AF_FILE_NO_SELECT ", "No Select", "checkbox", 1);					
			$this->file_attribute_flags_array[] = new attribute_flags("AF_FILE_NO_DELETE", "No Delete", "checkbox", 0);					
			$this->file_attribute_flags_array[] = new attribute_flags("AF_FILE_PHYSICAL_DELETE", "Physical Delete", "checkbox", 1);					
			$this->file_attribute_flags_array[] = new attribute_flags("AF_FILE_NO_AUTOPREVIEW", "No Auto Preview", "checkbox", 0);
			$this->file_attribute_flags_array[] = new attribute_flags("AF_FILE_POPUP", "Popup", "checkbox", 0);					
		}// end function


/****************************** START ACTIONS ******************************/
          
		function action_intro(&$handler){
               $db = atkGetDb();               
               $query = "SHOW TABLES";

               $db->query($query);
			$counter = 0;
               $data = array();
               while ($db->next_record())
               {
     			$table_name = $db->m_record["Tables_in_".$this->db_name];
     			//print $table_name." ($counter)<br>";
                    $data[] = $table_name;
                    $counter++;
               }// end while 
			if($counter==0){
				return "Currently there are no records in our database. Please come back soon.";
			}else{
                    //print count($data);
                    $this->print_tables_form($data);
               }
          }// end function

		function action_next(&$handler){
                //print "entering...<br>";
                if (is_array($this->m_postvars['atkselector'])) 
                { 
                     $selected_tables = array();
				 foreach ($this->m_postvars['atkselector'] as $selector) 
                     { 
                          $selected_tables[] = $selector;
                     } 
                     $this->find_relations($selected_tables);
                     $this->preview_form($selected_tables);
                } 
          }// end function

		/*5/20/06*/
		function action_generate_code(&$handler){
			$this->generate_code();
          }// end function

/****************************** END ACTIONS ******************************/


		// main function to generate code with assistance of many helper functions
		function generate_code(){
                //print "entering...<br>";
                $module_name = $this->m_postvars['module_name'];   
                $module_name = str_replace(" ","_",$module_name); // replace spaces from module name           
                $node_name = $this->m_postvars['node_name'];
			 $backup_files = $this->m_postvars['backup_files'];
			 $global_navigation = $this->m_postvars['global_navigation']; // added 1/19/07
                $languages = $this->m_postvars['languages']; // added 1/5/07
                $language_data = array();
                
                
			 if($backup_files){$backup_files=true;}
			 else{$backup_files = false;}
               
               // added 1/19/07
			 if($global_navigation){$global_navigation=true;}
			 else{$global_navigation = false;}

                $this->global_navigation = $global_navigation;
			 
			 $menu_nodes_array = array();	
			 $menu_nodes_labels = array();
			 $global_search_array = array();
			 
			 $log = "";			
			//print "backup_files: $backup_files<br>";
			 // create the module folder 
			 //$this->createModuleFolder($module_name);
			 	
			//print "module_name = $module_name<br>";
                if(is_array($node_name)){
			 	for($i=0; $i < count($node_name); $i++){ // iterate through every node
					$force_update_array 				= array();
					$attribute_position 				= 0;
					$selected_node 					= $node_name[$i];
					$selected_node_type 				= $this->m_postvars[$selected_node.'_node_type']; // regular, meta, tree
					$selected_node_options 				= $this->m_postvars[$selected_node.'_node_options']; // flags
					$selected_node_index_attribute 		= $this->m_postvars[$selected_node.'_index_attribute'];
					$selected_node_order_attribute 		= $this->m_postvars[$selected_node.'_order_attribute'];
					$selected_node_forceupdate_attribute 	= $this->m_postvars[$selected_node.'_forceupdate_attribute'];
					$selected_node_descriptor_attribute 	= $this->m_postvars[$selected_node.'_descriptor_attribute'];
					$selected_node_primary_attribute 		= $this->m_postvars[$selected_node.'_primary_field_attribute'];
                         $selected_node_color_attribute 		= $this->m_postvars[$selected_node.'_color_attribute'];
                         $selected_node_last_position 			= $this->m_postvars[$selected_node.'_last_position']; // position of last attribute. Used to know when to start o2m relations
					$selected_node_row_counter			= $this->m_postvars[$selected_node.'_node_counter']; // counter
					$selected_node_include_menu			= $this->m_postvars[$selected_node.'_node_include_menu']; // node menu inclusion
					$selected_node_include_messaging		= $this->m_postvars[$selected_node.'_node_include_messaging']; // node messaging inclusion
                         $selected_node_header_note			= $this->m_postvars[$selected_node.'_header_note']; // node header text
                         $selected_node_footer_note			= $this->m_postvars[$selected_node.'_footer_note']; // node header text
                         $selected_node_smart_search			= $this->m_postvars[$selected_node.'_node_smart_search']; // node smart_search
                         $selected_node_global_search			= $this->m_postvars[$selected_node.'_node_include_global_search']; // node global search
                         $selected_node_event_log			     = $this->m_postvars[$selected_node.'_node_event_log']; // node event log, added 9/30/06
                         $selected_node_intermediary			= $this->m_postvars[$selected_node.'_node_intermediary']; // node node_intermediary, added 10/21/06
					$selected_node_descriptor_link 	     = $this->m_postvars[$selected_node.'_node_descriptor_link']; // node descriptor link, added 11/16/06
					$selected_node_records_row 	          = $this->m_postvars[$selected_node.'_node_records_row']; // node # of records on admin mode, added 12/22/06
                         $selected_node_menu_label 	          = $this->m_postvars[$selected_node.'_node_menu_label']; // node manu label , added 1/19/07					
//print_r($this->m_postvars);
//die($selected_node.": ".$selected_node_menu_label);
					// set defaults for optional function inclusion flags
					
                         $this->function_includes_array["getMessage"]["state"] = false;
					$this->function_includes_array["recordActions"]["state"] = false;
					$this->function_includes_array["preAdd"]["state"] = false;
					$this->function_includes_array["preAddIntermediary"]["state"] = false;
					$this->function_includes_array["preAddIntermediary"]["values"] = "";
					$this->function_includes_array["preUpdate"]["state"] = false;
					$this->function_includes_array["initial_values"]["state"] = false;
					$this->function_includes_array["action_toggle"]["state"] = false;
					//$this->function_includes_array["active_display"]["state"] = false;
					//$this->function_includes_array["enabled_display"]["state"] = false;
					$this->function_includes_array["descriptor"] =false;
					$this->function_includes_array["action_toggle"]["field"]["active"]["state"] = false;
					$this->function_includes_array["action_toggle"]["field"]["enabled"]["state"] = false;
					$this->function_includes_array["action_toggle"]["field"]["approved"]["state"] = false;

                         $this->selected_node_descriptor_link["descriptorLink"]["state"] = false; // added 11/16/06

                         $this->selected_node_records_row = $selected_node_records_row;          
                         // initialize parent fields for this node
                         $this->parent_fields_array["state"] = false;
                         $this->parent_fields_array["field"]["parent_id"]["state"] = false;
                         $this->parent_fields_array["field"]["manager_id"]["state"] = false;
                         $this->fck_attributes_array = array(); // reset for each node, added 11/7/06
                         $this->add_validate = false; // added 12/30/06
                         if($selected_node_intermediary == 1) {
//print "<h2><font color=green>setting to true</font></h2>";
                              //$this->function_includes_array["preAddIntermediary"]["state"] = true;
                              $this->function_includes_array["preaddIntermediary"]["state"] = true;
//print "<h2><font color=green>sett to ".(($this->function_includes_array["preaddIntermediary"]["state"]==true)?"true":"false")."</font></h2>";
                         }                       
//print_r($this->m_postvars);
//print("<font color=red><b>$selected_node: selected_node_intermediary = $selected_node_intermediary</b> </font><br><br>");

                         // check descriptor link

                         if(!empty($selected_node_descriptor_link) && !empty($selected_node_descriptor_attribute)){
                              $this->selected_node_descriptor_link["descriptorLink"]["state"] = true;
                              $this->selected_node_descriptor_link["descriptorLink"]["field"] = $selected_node_descriptor_attribute;
                              $this->selected_node_descriptor_link["descriptorLink"]["mode"] = $selected_node_descriptor_link;
                         }// end if

                         // mark for global search inclusion
                         if($selected_node_global_search){
                              $global_search_array[] = $selected_node;
                         }
                         
                         // enable messaging functions
                         if($selected_node_include_messaging){
                              $this->function_includes_array["getMessage"]["state"] = true;
                         }
                         
					if(strlen($selected_node_row_counter)>0){
						$this->row_counter_detected = true;
					}
												
					if(is_array($selected_node_options)){ // separate node flags with pipes (|)
						if(array_search("NF_EDITAFTERCOPY",$selected_node_options)){ // check if edit after copy was selected and mark the signal to include appropriate function
							$this->function_includes_array["recordActions"]["state"] = true;							
						}

						$selected_node_options = implode(" | ",$selected_node_options);
					}

					if(is_array($selected_node_order_attribute)){ // separate order attributes with comma (,)
						$selected_node_order_attribute = implode(" , ",$selected_node_order_attribute);
					}

					if(is_array($selected_node_forceupdate_attribute)){ // place force update fields for this node on associative array
						for($f=0;$f<count($selected_node_forceupdate_attribute);$f++){
							$force_update_array[$selected_node_forceupdate_attribute[$f]] = true;
						}
					}

					
					//print "<hr>selected_node = $selected_node [*$selected_node_order_attribute*][$selected_node_type][[$selected_node_index_attribute]]($selected_node_options)<br>";
					$node_attributes = $this->m_postvars[$selected_node.'_attribute'];
					
					if(is_array($node_attributes)){ // get attribute options
						$node_include_array 		= array();
						$attribute_array 			= array();
						$attribute_relation_array 	= array();
						$attribute_tab_array 		= array();						
						$create_folder 			= false;
						$node_variables_array 		= array( // setup node's variables'
														"var \$table = \"".$selected_node."\";",
														"var \$primary_field = \"".$selected_node_primary_attribute."\";",
														"var \$module = \"".$module_name."\";",
												          "var \$node = \"".$selected_node."\";",
														"var \$index_field = \"".$selected_node_index_attribute."\";",
														"var \$order_field = \"".$selected_node_order_attribute."\";",
																								
												);						
						if(strlen($selected_node_descriptor_attribute)>0){ // only add descriptor if it exists
							$node_variables_array[] = "var \$descriptor_field = \"[".$selected_node_descriptor_attribute."]\";";
							$this->function_includes_array["descriptor"] = true;
						}// end if
						
						$this->getNodeIncludes(array("node_type"=>$selected_node_type),$node_include_array); // fill node includes
						
						for($j=0; $j< count($node_attributes); $j++){
							$options_array 		= array();
							$attribute_tab_variable 	= "";
							$attribute_name 		= $node_attributes[$j];
							$attribute_tab_name 	= $this->m_postvars[$selected_node.'*'.$attribute_name.'_tab_name'];
							$attribute_position 	= $this->m_postvars[$selected_node.'*'.$attribute_name.'_position'];							
							$attribute_position_orig	= $this->m_postvars[$selected_node.'*'.$attribute_name.'_position_orig'];
							$attribute_options 		= $this->m_postvars[$selected_node.'*'.$attribute_name.'_attribute_options']; // flags, etc
							$attribute_type 		= $this->m_postvars[$selected_node.'*'.$attribute_name.'_atk_attribute_type']; // eg: atkAttribute, atkFileAttribute, etc
							$attribute_db_type 		= $this->m_postvars[$selected_node.'*'.$attribute_name.'_db_attribute_type']; // eg: char, int, etc
							$attribute_relation 	= $this->m_postvars[$selected_node.'*'.$attribute_name.'_relation'];

                                   // add language for attribute. 1/5/07
                                   if (count($languages)>0) {
                                   	$language_data[$attribute_name] = $attribute_name;
                                   }

                                   if($attribute_type == $this->attributes["file"]){
                                        $attribute_file_folder = $this->m_postvars[$selected_node.'*'.$attribute_name.'_attribute_file_folder'];
                                        $attribute_file_folder = str_replace(" ","_",$attribute_file_folder); // replace spaces from folder name
                                        if(!empty($attribute_file_folder)){ // use provided folder name from user
                                             // for specific attribute
                                             $this->attachment_files_folder = $attribute_file_folder;
                                             // to create phisical files
                                             $this->attachment_files_folder_array[$attribute_file_folder] = $attribute_file_folder;
                                        }else{ // use default file folder name
                                             // for specific attribute
                                             $this->attachment_files_folder = $this->default_attachment_files_folder;
                                             // to create phisical files
                                             $this->attachment_files_folder_array[$this->default_attachment_files_folder] = $this->default_attachment_files_folder;
                                        }                                                                              
                                   }// end if
                                   
                                   if($attribute_type == $this->attributes["fck"]){
                                        $attribute_fck_toolbar = $this->m_postvars[$selected_node.'*'.$attribute_name.'_attribute_toolbar'];
                                        if(!empty($attribute_fck_toolbar)){ // use provided folder name from user
                                             // for specific attribute
                                             $this->fck_toolbar_type = $attribute_fck_toolbar;
                                        }else{ // use default file folder name
                                             $this->fck_toolbar_type = "Basic";
                                        }                                                                              
                                   }// end if
                                   
                                                                      
							if(strlen($attribute_position)<1){
								$attribute_position = $attribute_position_orig;
							}
							
							if($attribute_name == $this->function_includes_array["preAdd"]["field"]){
								$this->function_includes_array["preAdd"]["state"] = true;							
							}

							if($attribute_name == $this->function_includes_array["preUpdate"]["field"]){
								$this->function_includes_array["preUpdate"]["state"] = true;							
							}

							if($attribute_name == $this->function_includes_array["initial_values"]["field"]){
								$this->function_includes_array["initial_values"]["state"] = true;							
							}
					
							// check if we have a toggle field
                                   if(array_key_exists($attribute_name,$this->function_includes_array["action_toggle"]["field"])){
                                        //print "<h1><font color=red>detected: $attribute_name</font></h1>";
								if(!$this->function_includes_array["action_toggle"]["state"]){
                                             $this->function_includes_array["action_toggle"]["state"] = true;
                                        }
                                        // set state of the toggle field to true                                        
                                        $this->function_includes_array["action_toggle"]["field"][$attribute_name]["state"] = true;							
							}

                                   // check if field is parent field
                                   if(array_key_exists($attribute_name,$this->parent_fields_array["field"])){
                                        if(!$this->parent_fields_array["state"]){
                                             $this->parent_fields_array["state"] = true;
                                        }
                                        $this->parent_fields_array["field"][$attribute_name]["state"] = true;
                                   }// end if
					
							$options_array["attribute_type"] = $attribute_type;
				
							if(strlen($attribute_tab_name)>0){ // check for tab name
								$attribute_tab_variable = $this->getTabVariable($attribute_tab_name, $attribute_tab_array);
                                        $language_data["tab_".$attribute_tab_name] = $attribute_tab_name;// added 1/16/07
							}
							
							if(is_array($attribute_options)){ // separate attribute flags with pipes (|)
								$attribute_options = implode(" | ",$attribute_options);
							}

							if( (strstr($attribute_options,"AF_PRIMARY|AF_HIDE")) || (!is_array($attribute_relation))){ // attribute w/o relation
								$this->getNodeIncludes(array("attribute_type"=>$attribute_type),$node_include_array); // fill node includes
								$this->getAttributeCode(
													array(
														"attribute_type"=>$attribute_type,
														"attribute_db_type"=>$attribute_db_type,
														"attribute_name"=>$attribute_name,
														"attribute_flags"=>$attribute_options,
														"attribute_tab"=>$attribute_tab_variable,
														"module"=>$module_name,
														"node_name"=>$selected_node,
														"force_update_array"=>$force_update_array,
														"attribute_position"=>$attribute_position
													), 
													$attribute_array, $create_folder, 
                                                                 $attribute_relation_array, $node_include_array, count($node_attributes));
								
							}
							
							if(is_array($attribute_relation)){ // this attribute have relations
								for($r=0; $r< count($attribute_relation); $r++){ // iterate through relations
									list($source_table, $source_field, $destination_table, $destination_field, $relation_type) = explode("*", $attribute_relation[$r]);

                                             // add language for related attribute. 1/5/07
                                             if (count($languages)>0) {
                                             	$language_data[$source_field] = $source_field;
                                             }

									$this->getNodeIncludes( // get includes
													array(
														"relation_type"=>$relation_type,
														"attribute_type"=>$attribute_type,
													),
													$node_include_array); // fill node includes
									$relation_position = $attribute_position;
									if(   $relation_type == $this->relation_type_array["o2m"] || 
									      $relation_type == $this->relation_type_array["shuffle"] ||
									      $relation_type == $this->relation_type_array["bool"])
                                             {
                                                  $relation_position = $selected_node_last_position = $selected_node_last_position + 100;

                                                  // get m2o table of this o2m table for shuffle or many bool relations
                                                  if($relation_type == $this->relation_type_array["shuffle"] || $relation_type == $this->relation_type_array["bool"]){
                                                       // make the destination field the m2o relation table
                                                       $destination_field = $this->m_postvars[$selected_node.'*'.$attribute_name.'_'.$relation_type.'_relation'];
                                                  }                                                  
                                             }// end if
                                             
									// added 12/30/06 to include validate function
									// in case the AF_ONETOMANY_EMBED is oncluded on corresponding o2m relation
									// signals the acquisition of the next primary field value from the db
                                             if ($relation_type == $this->relation_type_array["m2o"]) {
         	                                        $this->add_validate = true;
                                             }// end if


									$this->getAttributeRelationCode( // get relation code
														array(
															"node_type"=>$selected_node_type,
															"relation_type"=>$relation_type,
															"source_table"=>$source_table,
															"source_field"=>$source_field,
															"destination_table"=>$destination_table,
															"destination_field"=>$destination_field,
															"attribute_name"=>$attribute_name,
															"attribute_flags"=>$attribute_options,
															"attribute_tab"=>$attribute_tab_variable,
															"attribute_position"=>$relation_position
														), 
														$attribute_relation_array);
								}// end for r (ralation)
								//$attribute_relation = implode(" |<br> ",$attribute_relation);
							}// end if(is_array($attribute_relation))
							
							//$attribute_position += 100;
							//$this->getNodeIncludes($node_include_array);	
							//print "<li>attribute_name = $attribute_name (".$attribute_options.") [type: $attribute_type/$attribute_db_type] - [rel: $attribute_relation]<br>";
							//print "&raquo;&raquo;&raquo; tab name = $attribute_tab_name <br>";
						}// end for attribute

						//print implode("<br>",$node_include_array);
						//print implode("<br>",$node_variables_array);
						//print implode("<br>",$attribute_tab_array);
						//print implode("<br>",$attribute_array);
						//print implode("<br>",$attribute_relation_array);

			
						$nodeCode = $this->getNodeCode(
									array(
											"node_name"             =>$selected_node,
											"node_type"             =>$selected_node_type,
											"includes_array"        =>$node_include_array,
											"node_variables_array"  =>$node_variables_array,
											"attributes_array"      =>$attribute_array,
											"tabs_array"            =>$attribute_tab_array,
											"relations_array"       =>$attribute_relation_array,
											"flags"                 =>$selected_node_options,
											"color_field"           =>$selected_node_color_attribute,
											"node_row_counter"      =>$selected_node_row_counter,
											"module_name"           =>$module_name,
											"header_note"           =>$selected_node_header_note,
											"footer_note"           =>$selected_node_footer_note,
											"smart_search"          =>$selected_node_smart_search,
											"event_log"             =>$selected_node_event_log,
											"include_menu"          =>$selected_node_include_menu
											
										)
									);

                              // store menu only if selecte by user
                              if(!empty($selected_node_include_menu)){
                                   $menu_nodes_array[] = $selected_node; // store node name for menu
                                   $menu_nodes_labels[] =  $selected_node_menu_label;
                              }
						

						// create the node file
						$log .= $this->createFile($module_name, "class.".$selected_node.".inc", $nodeCode, $backup_files);
												
						//print $nodeCode."\n";						
					}// end if(is_array($node_attributes)){ // get attribute options					
				}// end for($i=0; $i < count($node_name); $i++){ // iterate through every node

                    $menuCode = "";
				// generate menu code for module.inc file
                    if(count($menu_nodes_array)>0){
                         $menuCode = $this->getMenuCode($module_name, $menu_nodes_array, $global_search_array, $language_data, $menu_nodes_labels);

                         // create global nav node
		               if ($this->global_navigation) {
     					$nodeCode = $this->getNodeCode(
     								array(
     										"node_name"             =>$module_name."_menu",
     										"node_type"             =>"regular",
     										"node_variables_array"  =>array("var \$dispatch_url = \"dispatch.php\";","var \$module = \"$module_name\";","var \$node = \"".$module_name."_menu\";"),
     										"flags"                 =>"NF_NO_SECURITY",
     										"module_name"           =>$module_name,
     										"include_menu"          =>false,
     										"menu_nodes_array"      =>$menu_nodes_array,
     										"menu_labels_array"     =>$menu_nodes_labels,
     										"is_menu_node"          => true
     										
     									)
     								);
     
     					// create the node file
     				//	$log .= $this->createFile($module_name, "class.".$module_name."_menu".".inc", $nodeCode, $backup_files); off by bahtiar
                         }

                    }				


                    // get module code for config.module.inc
				$moduleConfigCode = $this->getModuleConfigCode($module_name);
		
				// create module menu file
				if(!empty($menuCode)){
                         $log .= $this->createFile($module_name, $this->module_menu_file, $menuCode, $backup_files);
                    }
                    
				
				// create module config file
				//$log .= "<h1>moduleConfigCode: $moduleConfigCode</h1><br>";
				if($moduleConfigCode){
					$log .= $this->createFile($module_name, $this->module_config_file, $moduleConfigCode, $backup_files);
				}				

				// check if we need to create any attachment folders
				if($this->attachment_files_detected || $this->row_counter_detected){
					$log .= $this->createModuleFolder($module_name, $backup_files);
     
     				// only create counter class of atk version is less than 5.6
                         if($this->row_counter_detected && (($this->atk_version < 5.6) || (empty($this->atk_version)))){
     					$rowCounterCode = $this->getRowCounterCode();
     					$log .= $this->createFile($module_name, $this->row_counter_file, $rowCounterCode, false);
     					
     				}

				}

  	               // process languages. 1/5/07
                    if (count($languages)>0 && count($language_data)>0) {
                     	$ldata = $this->process_language_data($language_data);
                     	$language_content = "";
                    	for ($i=0;$i< count($languages) ;$i++ ) {
                              $lng = $languages[$i];
                              $language_content = '<'.chr(63)."php\n\$".$lng.'=array('."\n".$ldata.');'."\n".chr(63).'>';

                              $log .= $this->createFile($module_name, $lng.".lng", $language_content, false, true);
//print $language_content."<hr>";                              
                         }// end for
                    }		
				// row counter
		
				$this->print_log($log);
				//print $menuCode."\n";
				//print $moduleConfigCode."\n";
			 }// end if(is_array($node_name)){		
		}// end function

          // added 1/5/07 to process languages data
          function process_language_data($language_array)
          { // BEGIN function process_language_data
               $return_value = "";
          	foreach ($language_array as $key=>$value) {
                    $tmp = substr($value,strlen($value)-3);
                    if(substr($value,strlen($value)-3) == "_id"){ // remove last _id text from display value
                         $value = substr($value,0,strlen($value)-3);
                    }
                    
                    $value = str_replace("_", " ", $value); // replace underscores with space
                    $return_value .= "\t".'"'.$key.'"=>"'.ucwords($value).'",'."\n";    
               }
               //die($return_value);
               return $return_value;
          } // END function process_language_data
          
          
		// prints the log of the code generated
          function print_log($log){

                $ui = &$this->getUi();
                $this->addStyle("style.css");
                $this->addStyle("recordlist.css");
                
                
                if (is_object($ui))
                {
                    $page = &$this->getPage();
                    $page->head(atktext("dashboard"));
                    $page->body();
                    $page->register_script("./atk/javascript/tools.js");
                    $page->register_script("./atk/javascript/recordlist.js");
                    $page->register_script("./atk/javascript/formselect.js");

                    
                    
                    $content = "
                         <table width=100% cellspacing=0> 
				            <!-- start header -->
				            <tr>					      
					      <th valign='middle' >                  Code Generated               </th>
				            </tr>
							<!-- end header -->
				            <!-- start footer -->
				            <tr>					      
						      <td valign='middle'> 
							 The ATK Code has been generated. Below you will find a log of the transaction. To continue you:<br><br>
							 <a href='javascript:history.back(-1)'>Go Back to Reconfigure Nodes</a> &nbsp;&nbsp;&nbsp;&nbsp;or&nbsp;&nbsp;&nbsp;&nbsp; <a href='".$this->dispatch_url."atkaction=intro'>Go to Table List</a>
							 </td>
				            </tr>
                      </table>
                    \n";
   

                    $content_box = $ui->renderBox(array("title"=>atktext("Complete"),
                                                         "content"=>$content)                                                        
                                                         );

                  $page->addContent($content_box);

                    $log_box = $ui->renderBox(array("title"=>atktext("Transaction Log"),
                                                         "content"=>$log)                                                        
                                                         );
			  $page->addContent($log_box);

               }// end if (is_object($ui))
          }// end function
          
		/* creates the physical files for module config, module name, and nodes
			Author: Jorge Garifuna - GariDigital.com - info@GariDigital.com
			Created date: 6/4/06

			@param $module_name 	- the name of the module 
			@param $entity_name 	- the name of the entity. It could be the name of the node, module.inc or config.module.inc 
			@param $entity_content 	- the content of the entity, for the most part the dynamic code that will be written into the file 
			@param $backup			- flag to back up file or not, dafaults to true to avoid overwriting files by accident 	
		*/
		function createFile($module_name, $entity_name, $entity_content, $backup=true, $language=false){
			$return_value = "<h3>Creating file for: $entity_name...</h3>\n";
			$parentDir = moduleDir($this->module).$this->destination_folder; // parent folder for module
			$dirName = $parentDir.$module_name;				// module folder
			$parentBackupDir = $parentDir.$this->backup_folder; 	// parent backup folder for config.module.inc file
			$backupDir = $dirName."/".$this->backup_folder; 		// backup folder
			$attributesDir = $dirName."/attributes"; // attributes folder
			$languagesDir = $dirName."/languages"; // attributes folder
			$file_name = $dirName."/".$entity_name; // module config file name

               if ($language) {
               		$file_name = $languagesDir."/".$entity_name; // module config file name
               }

			if($entity_name == $this->module_config_file){ // config.module.inc
				// replace ../ from file name to make sure that it saves on backup folder and not a level up
				$old_file_name = $parentBackupDir."/".str_replace("../","",$entity_name).".old"; // backup file name
			}else{
				$old_file_name = $backupDir."/".$entity_name.".old"; // backup file name
			}
			
			
			// make sure module folder exists, if not request for creation
			if((!file_exists($parentDir) || !file_exists($dirName)) || ($backup && !file_exists($backupDir)) || ($language && !file_exists($languagesDir))){
				$return_value .= $this->createModuleFolder($module_name, $backup,$language);
			}// end if
			
			// attempt to backup existing file
			if (file_exists($file_name) && $backup){
				if (is_file($file_name)){
						//$old_file_name = $file_name.".old";
						
						if (!rename($file_name, $old_file_name)){
							$old_file_name .= "_".date('mdYhms');
							if (!rename($file_name, $old_file_name)){
									print "<font color=red>ERROR: Could not backup file $file_name into $old_file_name. Exiting to maintain file integraty</font><br>\n\n";
									exit(1) ;
							}else{
								$return_value .= "<font color=blue>File ($file_name) successfully backed up into $old_file_name...</font> <br>\n\n";
							}// end if
						}else{
							$return_value .= "<font color=blue>File ($file_name) successfully backed up into $old_file_name... </font><br>\n\n";
						}// end if
				}else{
					print "<font color=red>ERROR: $file_name is not a file. Quiting to protect file integrity</font><br>\n\n";
					exit(2) ;
				}// end if (is_file($file_name)){
			}// end if (file_exists($file_name) && $backup){
			
			if (!($theStream = @fopen($file_name, 'w'))){
				print "<font color=red>ERROR: Could not open file ($file_name) for writing. Quiting to protect file integrity</font><br>\n\n";
				exit(3) ;
			}

			// Write node code to our opened file.
			if (fwrite($theStream, $entity_content) === FALSE) {
			  print "<font color=red>ERROR: Cannot write to file ($file_name). Quiting</font><br>\n";
			  exit(4);
			}
			   
			$return_value .= "<font color=green><b>Success, file ($file_name), created... </b></font><hr>\n";
			   
			fclose($theStream);
			
			return $return_value;
		}// end createFile function
		

		// create the module directory for all nodes to reside on 
		function createModuleFolder($module_name, $backup=false, $language=false){
			$return_value = "";
			// check module name
			if(strlen($module_name)<1){
				print "<font color=red>ERROR: module name is empty. Please enter a module name and try again</font><br>\n";
				exit(0);
			}
			
			$parentDir = moduleDir($this->module).$this->destination_folder; // parent folder name of module
			$dirName = $parentDir.$module_name; // folder name of module
			$attachmentDir = $dirName."/".$this->attachment_files_folder; // attachement folder 
			$parentBackupDir = $parentDir.$this->backup_folder; // parent backup folder for config.module.inc file
			$backupDir = $dirName."/".$this->backup_folder; // backup folder
			$attributesDir = $dirName."/attributes"; // attributes folder
			$languagesDir = $dirName."/languages"; // languages folder
			
		      // create the parent directory for this module
		    	if(!file_exists($parentDir)){
		    		if(!mkdir($parentDir)){
		    			print "<font color=red>ERROR: Could not create directory,$parentDir</font> <br>\n\n";
		    			exit(0);
		    		}else{
				    $return_value .= "<font color=green>new directory, $parentDir, created successfully...</font><br>\n\n";
				}	    				
		    	}else{
			    	$return_value .= "<font color=green>directory, $parentDir, already exists...</font><br>\n\n";
			}

			// create directory for module
	    		if(!file_exists($dirName)){
		    		if(!mkdir($dirName)){
		    			print "<font color=red>ERROR: Could not create directory,$dirName</font><br>\n\n";
		    			exit(0);
		    		}else{
				    $return_value .= "<font color=green>new directory, $dirName, created successfully...</font><br>\n\n";
				}	    		
	    		}else{
	    			$return_value .= "<font color=blue>directory, $dirName, already exists...</font><br>\n\n";			    
			}			
			
			// do we have any attachment folder to create
			if($this->attachment_files_detected){
				// create directory for attachment
				// user may have specified other folder than the default, so we need to loop through all available options
                    //for($i=0; $i< count($this->attachment_files_folder_array); $i++){
                    foreach ($this->attachment_files_folder_array as $key => $file_folder_name){
     		    		$attachmentDir = $dirName."/".$file_folder_name; //$this->attachment_files_folder_array[$i]; // attachement folder
                         if(!file_exists($attachmentDir)){
     			    		if(!mkdir($attachmentDir)){
     			    			print "<font color=red>ERROR: Could not create directory,$attachmentDir</font><br>\n\n";
     			    			exit(0);
     			    		}else{
     					    $return_value .= "<font color=green>new directory, $attachmentDir, created successfully...</font><br>\n\n";
     					}	    		
     		    		}else{
     		    			$return_value .= "<font color=blue>directory, $attachmentDir, already exists...</font><br>\n\n";			    
     				}
                    }// end for					
			}

			// do we need a backup folder?
			if($backup){
				// create directory for config.module.inc backup
		    		if(!file_exists($parentBackupDir)){
			    		if(!mkdir($parentBackupDir)){
			    			print "<font color=red>ERROR: Could not create directory,$parentBackupDir</font><br>\n\n";
			    			exit(0);
			    		}else{
					    $return_value .= "<font color=green>new directory, $parentBackupDir, created successfully...</font><br>\n\n";
					}	    		
		    		}else{
		    			$return_value .= "<font color=blue>directory, $parentBackupDir, already exists...</font><br>\n\n";			    
				}					

				// create directory for node backup
		    		if(!file_exists($backupDir)){
			    		if(!mkdir($backupDir)){
			    			print "<font color=red>ERROR: Could not create directory,$backupDir</font><br>\n\n";
			    			exit(0);
			    		}else{
					    $return_value .= "<font color=green>new directory, $backupDir, created successfully...</font><br>\n\n";
					}	    		
		    		}else{
		    			$return_value .= "<font color=blue>directory, $backupDir, already exists...</font><br>\n\n";			    
				}					
			}
               
               if($language){ // added 1/5/07
     			// create directory languages
     	    		if(!file_exists($languagesDir)){
     		    		if(!mkdir($languagesDir)){
     		    			print "<font color=red>ERROR: Could not create directory, $languagesDir</font><br>\n\n";
     		    			exit(0);
     		    		}else{
     				    $return_value .= "<font color=green>new directory, $languagesDir, created successfully...</font><br>\n\n";
     				}	    		
     	    		}else{
     	    			$return_value .= "<font color=blue>directory, $languagesDir, already exists...</font><br>\n\n";			    
     			}					
               }// end  language
               
			// do we have any attachment folder to create
			if($this->row_counter_detected){
				// create directory for attributes
		    		if(!file_exists($attributesDir)){
			    		if(!mkdir($attributesDir)){
			    			print "<font color=red>ERROR: Could not create directory,$attributesDir</font><br>\n\n";
			    			exit(0);
			    		}else{
					    $return_value .= "<font color=green>new directory, $attributesDir, created successfully...</font><br>\n\n";
					}	    		
		    		}else{
		    			$return_value .= "<font color=blue>directory, $attributesDir, already exists...</font><br>\n\n";			    
				}					
			}


			return $return_value;
		}// end function
		
		// obtains the module config code for the file config.modules.inc
		// returns false if file does not need to be updated because module already there
		function getModuleConfigCode($module_name){
			$return_value = false; // default to return nothing, assuming the module is already part of the list
			$new_module_code = "module(\"".$module_name."\");";
			$file_name = moduleDir($this->module).$this->module_config_file;
				//print "file_name: $file_name<br>";
			$data = file($file_name);
			
			$total_lines = count($data);
			$found = false;
			$end_line = -1;
			for($i=0; $i < $total_lines; $i++){
				$line = chop($data[$i]);
                    // start: added 7/31/06
				if(empty($line)){
                         unset($data[$i]); // remove blank lines
                         continue;
                    }
                    // end: added 7/31/06
                    
				//print "$line<br>\n"; 
				if(strstr($line,$new_module_code)){
					$found = true;
					break;
				}
				
				if(strstr($line,"?>")){
					$end_line = $i;
					break;
				}
			} // end for
		
			if(!$found && $end_line != -1){ // not found so add new module
				$data[$end_line] = "\t".$new_module_code." // added by ATK Code Generator on ".date("M d, Y").". Author: Jorge Garifuna (http://www.GariDigital.com) <info@GariDigital.com>\n";
				$data[] = "?>"; 
				//$return_value = implode("\n",$data);
				$return_value = implode("",$data); // added 7/31/06
			}
			return $return_value; // no need to update
		}// end function

		function getRowCounterCode(){
			$return_value = "<?php
				atkimport(\"atk.attributes.atkdummyattribute\"); 
				class counterAttribute extends atkDummyAttribute 
				{ 
				   function counterAttribute(\$name, \$flags=0) 
				   { 
				      \$this->atkDummyAttribute(\$name, '', \$flags|AF_HIDE_VIEW|AF_HIDE_EDIT|AF_HIDE_ADD); 
				   } 
				
				   function display() 
				   { 
				      static \$s_counter = 0; 
				      \$node = &\$this->m_ownerInstance; 
				      return \$node->m_postvars[\"atkstartat\"]+(++\$s_counter); 
				   } 
				} 
						
				?>
		";
			return $return_value;
		}

		// obtains the menu code for the module.inc file
		function getMenuCode($module_name, $menu_nodes_array, $global_search_array,&$language_data, $menu_nodes_labels){
			$return_value = "";		
			$menu_code = "";
			$module_menu_permission_code = "";
			$permission_code = "";
			
			$total_nodes = count($menu_nodes_array);

               $permission_tabs = "\t\t\t\t\t\t\t\t\t\t";
			for($i=0;$i<$total_nodes; $i++){ // get menu and permission code per node for inclusion later on
				$node_name = $menu_nodes_array[$i];
				/* commented 1/19/07 in favor of using $menu_nodes_labels as provided by user
                    $node_label = str_replace("_"," ", $node_name); // replace all underscores with spaces
				$node_label = ucwords(strtolower($node_label));  // make first letters uppper case
                    */
                    
                    $node_label = $menu_nodes_labels[$i]; // added 1/19/07
                    
                    $language_data["menu_".$node_label] = $node_label;// added 1/16/07
                    			
				$menu_code .= "\t\$this->menuitem(\"".$node_label."\", dispatch_url(\$this->module.\".".$node_name."\", \"admin\"), \$main_label,array(\$this->module.\".".$node_name."\",\"admin\",\$this->module.\".".$node_name."\",\"add\",\$this->module.\".".$node_name."\",\"edit\",\$this->module.\".".$node_name."\",\"delete\"));\n";
				// use this hardcoded permissions until implemented in interface
		          $togle_action = "";
                    if($this->function_includes_array["action_toggle"]["state"]){
     				$toggle_fields_array = array_keys($this->function_includes_array["action_toggle"]["field"]); 

     				for($t=0;$t < count($toggle_fields_array); $t++){
                              $toggle_field_name = $toggle_fields_array[$t];
     
                              // if this toggle field is present on this table, then include the action/display functions for it
                              if($this->function_includes_array["action_toggle"]["field"][$toggle_field_name]["state"]){
                                   $togle_action .= ",\"".$toggle_field_name."\"";
                              }// end toggle field inclusion                    
                         }// end for
                        //$togle_action = ",\"toggle\"";
                    }
				$permission_code .= "\tregisterNode(\$this->module.\".".$node_name."\", array(\"admin\", \"add\", \"edit\", \"delete\"".$togle_action."));\n";
				
				// add module permission for main menu
                    $module_menu_permission_code .= $permission_tabs."\$this->module.\".".$node_name."\",\"admin\",";
				$module_menu_permission_code .= "\$this->module.\".".$node_name."\",\"add\",";
				$module_menu_permission_code .= "\$this->module.\".".$node_name."\",\"edit\",";
				$module_menu_permission_code .= "\$this->module.\".".$node_name."\",\"delete\",\n";
			}
		
		     // wrap module permission into array 
               if(!empty($module_menu_permission_code)){
                    $module_menu_permission_code = "array(\n".$module_menu_permission_code."\n".$permission_tabs.")";
               }

               $global_search_code = $this->getGlobalSearchCode($global_search_array);
               
			$return_value = $this->generateMenuCode($module_name, $menu_code, $permission_code, $module_menu_permission_code, $global_search_code);		

			return $return_value;
		}// end function getMenuCode

          function getGlobalSearchCode($global_search_array){
               $return_value = "";
               $total_items = count($global_search_array);
               
               if($total_items > 0){
                    $tabs1 = "\t\t";
                    $tabs2 = "\t\t\t";
                    $return_value = "
     /* start top search implementation */
     function search(\$expression){
          \$res = array();
            \n";
                    for($i=0; $i < $total_items; $i++){
                         $node_name = $global_search_array[$i];
                         
                         if(substr($node_name,-1,1) != "s"){ // if last char of node name is not "s" then pluratize label
                              $node_label = $node_name."s";
                         }else{
                              $node_label = $node_name;
                         }
                         $return_value .= $tabs2."\$".$node_name."_node = atkGetNode(\$this->module.\".".$node_name."\");\n";        
                         $return_value .= $tabs2."\$res[\"".$node_label."\"] = \$this->recLinks(\$".$node_name."_node->searchDb(\$expression), \$this->module.\".".$node_name."\");\n\n";
                    }// end for     
               
               // close search() function
               $return_value .= "
           return \$res;
       }// end function
     
       function recLinks(\$recordset, \$nodetype)
         {
     
           \$res = array();
     
           \$node = atkGetNode(\$nodetype);
           if (\$node->allowed(\"view\"))
           {
             for(\$i=0, \$_i=count(\$recordset); \$i<\$_i; \$i++)
             {
               \$item[\"title\"] = \$node->descriptor(\$recordset[\$i]);
               \$item[\"url\"] = dispatch_url(\$nodetype, \"view\", array(\"atkselector\"=>\$node->primaryKey(\$recordset[\$i])));
               \$res[] = \$item;
             }
           }
     
           return \$res;
       } // end function
          /* end top search implementation */



               \n";
               }// end if

               
               return  $return_value;
          }// end function
          
		
		function generateMenuCode($module, $menu_list, $permission_list, $module_menu_permission_code, $global_search_code){
			$module_label = str_replace("_"," ", $module); // replace all underscores with spaces
			$module_label = ucwords(strtolower($module_label));  // make first letters uppper case

			$return_value = "<?php
  
".$this->getAuthorNote($module, "menu")."
  class mod_".$module." extends atkModule
  {           
    var \$module = \"".$module."\";
    
    function getMenuItems()
    {          
      \$main_label = \"".$module_label."\";
      // the last parameter hides the main module menu for this node from users without access
	 \$this->menuitem(\$main_label,\"\",\"main\",".$module_menu_permission_code."); 
       
       // the last parameter hides the node menu from users without access
".$menu_list." 

      /** 
       * The following line adds the new profiles menuitem.
       * New here is the 4th parameter to the menuitem() call. This parameter
       * makes the menuitem disappear, if the current user does not have the 
       * 'admin' privilege on the profile node.
       * If you don't pass this parameter, the menuitem is always visible, and
       * the user will get an 'access denied' message when clicking the item
       * anyway. (you can try this by accessing the 'lesson1' menuitems when
       * logged in as one of the employees.)
       */
      //\$this->menuitem(\"profiles\", dispatch_url(\"company.profile\", \"admin\"), \"company\", array(\"company.profile\", \"admin\"));
           
      //\$this->menuitem(\"-\", \"\", \"company\");
      
      //\$this->menuitem(\"modulesource\", moduleSourceUrl(\"company\"), \"company\");
    }
    
    /**
     * This lesson contains security profiles. This means that no longer all
     * users may execute all actions. The administrator user still can, but
     * other users need privileges.
     * With this method, the system is instructed of the privileges that
     * exist. Usually, this corresponds to the actions that can be done
     * on a node.
     */
    function getNodes()
    {    
      /**
       * In this case, we tell the system that the nodes
       * have 'admin', 'add', 'edit' and 'delete' privileges.
       */
".$permission_list."
      //registerNode(\"company.profile\", array(\"admin\", \"add\", \"edit\", \"delete\", \"grantall\"));

    }

".$global_search_code."    
  }// end class

?>";
			
			return $return_value;
		
		}// end function 
          // adds table to database to store smar search items
          function add_smart_search_table(){
               // only create table if it does not exists
               $db = atkGetDb();               
               $query = "CREATE TABLE IF NOT EXISTS atk_smartsearchcriteria 
                         ( 
                           nodetype VARCHAR(100) NOT NULL, 
                           name VARCHAR(100) NOT NULL, 
                           criteria TEXT NOT NULL, 
                           PRIMARY KEY (nodetype, name) 
                         ); ";

               $db->query($query);      
          }// end function

          // adds table to database to store event log items
          function add_event_log_table(){
               // only create table if it does not exists
               $db = atkGetDb();               
               $query = "CREATE TABLE IF NOT EXISTS atkeventlog 
                         ( 
                               id INT(10) NOT NULL auto_increment,
                               userid INT(10),
                               stamp DATETIME,
                               node VARCHAR(100),
                               action VARCHAR(100),
                               primarykey VARCHAR(255),
                               PRIMARY KEY  (id)
                         ); ";

               $db->query($query);      
          }// end function
		
          		
		// obtains the node code
		function getNodeCode($properties_array){

			$name 				= $properties_array["node_name"];
			$node_type 			= $properties_array["node_type"];			
			$includes_array 		= $properties_array["includes_array"];
			$node_variables_array 	= $properties_array["node_variables_array"];
			$attributes_array 		= $properties_array["attributes_array"];
			$tabs_array 			= $properties_array["tabs_array"];
			$relations_array 		= $properties_array["relations_array"];
			$flags 				= $properties_array["flags"];
			$color_field			= $properties_array["color_field"];
			$node_row_counter		= $properties_array["node_row_counter"];
			$module_name			= $properties_array["module_name"];
			$header_note			= $properties_array["header_note"];
			$footer_note			= $properties_array["footer_note"];
			$smart_search			= $properties_array["smart_search"];
			$event_log			= $properties_array["event_log"];
			$include_menu			= $properties_array["include_menu"];
			$is_menu_node			= $properties_array["is_menu_node"];
			$menu_nodes_array		= $properties_array["menu_nodes_array"];
		     $menu_labels_array		= $properties_array["menu_labels_array"];

               if ($is_menu_node) { // hide controls for menu node                    
                    $this->function_includes_array["preAdd"]["state"] = false;			
                    $this->function_includes_array["preUpdate"]["state"] = false;
                    $this->function_includes_array["getMessage"]["state"] = false;
                    $this->function_includes_array["initial_values"]["state"] = false;
                    $this->function_includes_array["action_toggle"]["state"] = false;
                    $this->function_includes_array["descriptor"] = false;
                    $this->function_includes_array["adminHeader"] = false;
                    $this->function_includes_array["adminFooter"] = false;
                    $this->function_includes_array["recordActions"]["state"] = false;
                    $this->function_includes_array["editPage"] = false;
                    $this->function_includes_array["viewPage"] = false;
                    $this->add_validate = false;
                    $this->selected_node_descriptor_link["descriptorLink"]["state"] = false;
               }// end if
			
			$ftabs = "\t\t"; // function tabs
			$atabs = "\t\t\t"; // attribute tabs

					
			// node includes
			$return_value = "<?php\n\n";
			$return_value .= $this->getAuthorNote($name);
			if(is_array($includes_array)){$return_value .= "\n\n".implode("\n",$includes_array)."\n\n";}

			

			// include row counter
			if(strlen($node_row_counter)>0){
				if(($this->atk_version < 5.6) || (empty($this->atk_version))){
                         $return_value .= "useattrib(\"".$module_name.".counterattribute\");\n";
                    }else{
                         $return_value .= "useattrib(\"atkrowcounter\");\n";
                    }
                    
			}// end if
			
			// node header
			if($node_type == "regular"){ // regular node
				$return_value .= "class ".$name." extends atkNode{\n";
			}else if($node_type == "meta"){ // meta node
				$return_value .= "class ".$name." extends atkMetaNode{\n";
			}else if($node_type == "tree"){ // tree node
				$return_value .= "class ".$name." extends atkTreeNode{\n";
			}
			
			// node global variables
			$return_value .= "\n".$ftabs.implode("\n".$ftabs,$node_variables_array)."\n\n";

               // added menu object if glbal navigation, added 1/19/07
               if ($this->global_navigation && !empty($include_menu)) {
               	$return_value .= "\t\tvar \$menu;\n\n";
               }
			
			// initiate constructor
			$return_value .= $ftabs."// start constructor\n";
			// $return_value .= $ftabs."function ".$name."(){\n"; // bahtiar2024
			$return_value .= $ftabs."function __construct(){\n";

			// tabs labels
			if(is_array($tabs_array)){
				$return_value .= $atabs."// tabs labels\n";
				foreach ($tabs_array as $key => $value){
					$return_value .= $atabs.$key." = \"".$value."\";\n"; //implode("\n".$atabs,$tabs_array)."\n";
				}// end foreach				
				$return_value .= "\n";
			}

		

			// constructor call to parent node 
			$return_value .= $atabs."// call constructor parent\n";
			if($node_type == "regular"){ // regular node
				// $return_value .= $atabs."\$this->atkNode(\"".$name."\",".$flags.");\n";                            	               
				$return_value .= $atabs."\$this->atkNode(\$this->node,".$flags.");\n";
			}else if($node_type == "meta"){ // meta node				
				$return_value .= $atabs."\$this->atkMetaNode(array(\"flags\"=>".$flags."));\n";
			}else if($node_type == "tree"){ // tree node				
				$return_value .= $atabs."\$this->atkTreeNode(\"".$name."\",".$flags.");\n";
			}
			$return_value .= "\n";

               // added menu object if global navigation, added 1/19/07
               if ($this->global_navigation  && !empty($include_menu)) {
                    $return_value .= $this->getExternalModuleCode($module_name."_menu","\$this->menu = getNode(\$this->module.\".".$module_name."_menu\");");
                    //$return_value .= "\t\t\t \n\n";
               }

               // event log
               // smart_search 
               if($event_log){
                    $this->add_event_log_table();
                    $return_value .= $atabs."//event log\n".$atabs."\$this->addListener(atknew(\"atk.utils.atkeventlog\"));\n";                    
               }
			// attributes
			$return_value .= $atabs."// node attributes\n";

			// include row counter
			if(strlen($node_row_counter)>0){
				if(($this->atk_version < 5.6) || (empty($this->atk_version))){
				      $return_value .= $atabs."\$this->add(new counterAttribute(\"#\"),NULL,1);\n";
				}else{ // we assume atk version 5.6 or greater
                         $return_value .= $atabs."\$this->add(new atkRowCounter(\"#\"),NULL,1);\n";
                    }
                    			
			}// end if


			if(is_array($attributes_array)){$return_value .= $atabs.implode("\n".$atabs,$attributes_array)."\n\n";}

			

               // smart_search 
               if($smart_search){
                    $this->add_smart_search_table();
                    $return_value .= $atabs."//smart search\n".$atabs."\$this->setExtendedSearchAction('smartsearch');\n";
                    
               }
			// relations
			if(($node_type == "regular") || ($node_type == "tree")){ // regular/tree node relation
				$return_value .= $atabs."// relations\n";
				
				
				if(is_array($relations_array)){$return_value .= $atabs.implode("\n".$atabs,$relations_array)."\n"; }
				
				$return_value .= "\n";
				$return_value .=$this->getConstructorEnding($atabs, $ftabs,(($is_menu_node)?false:true));  // end constructor here
			}else if($node_type == "meta"){ // meta node				
				$return_value .= "\n";
				$return_value .=$this->getConstructorEnding($atabs, $ftabs); // end constructor here
				$return_value .= $ftabs."function meta(&\$policy){\n".$atabs.implode("\n".$atabs,$relations_array)."\n".$ftabs."}// end function\n";
			}

               // include admin action
               if (!empty($this->selected_node_records_row) && $this->selected_node_records_row != "none") {
                    if ($this->global_navigation  && !empty($include_menu)) {
                         // add view action for global navigation menu
                         $gncode = $this->getExternalModuleCode($module_name."_menu","\$this->menu->load_my_scripts(\"admin\");");     
                    }else{
                         $gncode= "";
                    }
                    
                    if (!$is_menu_node) {
               	    $return_value .= $this->get_master_admin($this->selected_node_records_row, $gncode);                    	
                    }

               }

               // include global menu load script function, added 1/19/07
               if ($is_menu_node) {
                    $return_value .= $this->get_master_menu_load_script();	
                    $return_value .= $this->get_master_menu_js_code($menu_nodes_array, $menu_labels_array);
               }

               // added menu view action function if global navigation, added 1/19/07
               if ($this->global_navigation  && !empty($include_menu)) {
                    // add view action for global navigation menu
                    $gncode = $this->getExternalModuleCode($module_name."_menu","\$this->menu->load_my_scripts(\"view\");");     
                    $return_value .= $this->get_master_view($gncode);

                    // add add action for global navigation menu
                    $gncode = $this->getExternalModuleCode($module_name."_menu","\$this->menu->load_my_scripts(\"add\");");     
                    $return_value .= $this->get_master_add($gncode);

                    // add edit action for global navigation menu
                    $gncode = $this->getExternalModuleCode($module_name."_menu","\$this->menu->load_my_scripts(\"edit\");");     
                    $return_value .= $this->get_master_edit($gncode);

               }

               // added menu view action function if global navigation, added 1/19/07

               if (!$is_menu_node) {
                    // include save funcion 
                    $return_value .= $this->get_master_save();
               }
               // include display function for descriptor. added 11/16/06
               if($this->selected_node_descriptor_link["descriptorLink"]["state"]){
                    $return_value .= $this->get_descriptor_display($this->selected_node_descriptor_link["descriptorLink"]["field"], $this->selected_node_descriptor_link["descriptorLink"]["mode"]);
               }// end if
               
               // include display function for fck
               if(count($this->fck_attributes_array) > 0){
                    foreach ($this->fck_attributes_array as $aname => $avalue){
                         $return_value .= $this->get_display_function($aname);          
                    }
               }// end if
//print "<h1>state: ".(($this->function_includes_array["preaddIntermediary"]["state"]==true)?"true":"false")." && !empty(".$this->function_includes_array["preAddIntermediary"]["values"].")</h1>";               
			// include optional functions
               $include_tracker = false;
			if($this->function_includes_array["preAdd"]["state"]){
				$return_value .= $this->get_preadd();
                    $include_tracker = true;
			}else if($this->function_includes_array["preaddIntermediary"]["state"] && !empty($this->function_includes_array["preAddIntermediary"]["values"])){
                    // make sure to include relations on intermediary table to get sides id's prior to table insert: table/primary_id
				$return_value .= $this->get_preaddIntermediary($this->function_includes_array["preAddIntermediary"]["values"]);
                    $include_tracker = true;
			}

			if($this->function_includes_array["preUpdate"]["state"]){
				$return_value .= $this->get_preupdate();
                    $include_tracker = true; 
			}

               if($include_tracker){
                    $return_value .= $this->get_trackrecord();                                   
               }


			if($this->function_includes_array["getMessage"]["state"]){
				$return_value .= $this->get_messaging_code();
			}

			if($this->function_includes_array["initial_values"]["state"]){
				$return_value .= $this->get_initial_values();
			}

               // include action/display functions for toggle fields if availble
			if($this->function_includes_array["action_toggle"]["state"]){
				$toggle_fields_array = array_keys($this->function_includes_array["action_toggle"]["field"]); 
                    $tcounter = 0;
                    $toggle_checked_code = ""; 	// used for the checked color code of the toggle field
                    $toggle_unchecked_code = ""; 	// used for the unchecked color code of the toggle field
				for($t=0;$t < count($toggle_fields_array); $t++){
                         $toggle_field_name = $toggle_fields_array[$t];

                         // if this toggle field is present on this table, then include the action/display functions for it
                         if($this->function_includes_array["action_toggle"]["field"][$toggle_field_name]["state"]){
						$subsequent_else = ""; // used for else of subquent toggle fields
						if($tcounter > 0){
			                    $subsequent_else = "else ";
						} // end if

						// make sure check color exists
						if((strlen($this->function_includes_array["action_toggle"]["field"][$toggle_field_name]["checked_color"])>0) || (strlen($this->function_includes_array["action_toggle"]["field"][$toggle_field_name]["unchecked_color"])>0)){
							$toggle_checked_code   .= $atabs.$subsequent_else."if(\$record['".$toggle_field_name."'] ==0){\$color = \"".$this->function_includes_array["action_toggle"]["field"][$toggle_field_name]["unchecked_color"]."\";}\n"; 
			                    $toggle_unchecked_code = $atabs."else if(\$record['".$toggle_field_name."'] ==1){\$color = \"".$this->function_includes_array["action_toggle"]["field"][$toggle_field_name]["checked_color"]."\";}\n".$toggle_unchecked_code;												
						}// end if

						$tcounter++;
                              $return_value .= $this->get_toggle($toggle_field_name);                         
                         }// end toggle field inclusion                    
                    }// end for

				// obtain function for color control display field                    
                    if(strlen($toggle_checked_code)>0 && strlen($color_field)>0){
					$return_value .= $this->get_color_toggle_display($color_field, $toggle_checked_code.$toggle_unchecked_code);				
				}// end if
			}// end toggle field

			if($this->function_includes_array["descriptor"]){
				$return_value .= $this->get_descriptor();
			}

               // added 12/30/06
               if ($this->add_validate) {
                    $return_value .= $this->get_validation();               	
               }

			if($this->function_includes_array["adminHeader"]){
				$return_value .= $this->get_admin_header($name, $header_note);
			}

			if($this->function_includes_array["adminFooter"]){
				$return_value .= $this->get_admin_footer($footer_note);
			}

			if($this->function_includes_array["recordActions"]["state"]){
				$return_value .= $this->get_record_actions();
			}

			$addPageFooter = false; // flag to add general function for editPage and viewPage
			if($this->function_includes_array["editPage"]){
				$return_value .= $this->get_edit_page();
				$addPageFooter = true;
			}

			if($this->function_includes_array["viewPage"]){
				$return_value .= $this->get_view_page();
				$addPageFooter = true;
			}

			if($addPageFooter){
				$return_value.= $this->get_page_footer();			
			}
			
			$return_value .= "\n}// end node ".$name."\n\n?>"; 
						
			return $return_value;
			
		}// end if

          
		/* START INCLUSION OF OPTIONAL FUNCTIONS */

          function getExternalModuleCode($node_name,$code){
			$return_value = "
               if(nodeExists(\$this->module.\".".$node_name."\")){ // only add if node exists
          	    $code
          	}// end if menu
               ";
               return $return_value;
          
          }// end function

          function get_validation(){
			$return_value = "

          // validate the record
         function validate(&\$record, \$mode, \$ignoreList=array()){
               if (\$mode == \"add\") {// get next id, if add mode
                    \$this->getMyNextId(\$record);               	
               }
               return parent::validate(\$record, \$mode, \$ignoreList);     
         }// end function

	    /* 
               get the next id from the table for this record, only if they are not present on the given record
	    
	                   Jorge Garifuna - info@garidigital.com
               created: 9/12/06
          */
          function getMyNextId(&\$record){
               // only get the next id if passed id value is empty
               if (empty(\$record[\$this->primary_field]) && empty(\$record[\"atkprimkey\"])) {
                    \$db = atkGetDb(); 
                    // get the id of the last record that was inserted
                    \$query = \"select max(\".\$this->primary_field.\") as last_id from \".\$this->table;
                    \$rows = \$db->getrows(\$query);               
                    if(count(\$rows)>0){ 
                         \$id = (\$rows[0][\"last_id\"] + 1);
                         \$record[\$this->primary_field] = \$id;
                         \$record[\"atkprimkey\"] = \$this->node.\".\".\$this->primary_field.\"='\$id'\";                                           
                    }// end function                       
               }// end if
          }// end function

               ";
               return $return_value;
                    
          }// end real function

          function get_display_function($aname){
			$return_value = "

          function ".$aname."_display(\$record){
               return \$record[\"$aname\"];
          }// end function
               ";
               return $return_value;
                    
          }// end real function
          
          // obtains the node messaging code
          function get_messaging_code(){
               $return_value = "
          /* START NODE MESSAGING */
          
          // this function is automatically called after a record has been updated
         function postUpdate(\$record){                   
               // notify the user that the record has been updated
               \$this->printMessage(\"Record, \".\$record[\$this->index_field].\", Updated Successfully\");
               return true;
         }

          // this function is automatically called after a record has been deleted			
         function postDelete(\$record){                   
               // notify the user that the record has been deleted
               \$this->printMessage(\"Record, \".\$record[\$this->index_field].\", Deleted Successfully\");
               return true;
         }

          // this function is automatically called after a record has been added
         function postAdd(\$record, \$mode = \"add\"){                   
               \$this->getMyLastId(\$record);
               // notify the user that the record has been created
               \$this->printMessage(\"Record, \".\$record[\$this->index_field].\", Created Successfully\");
               return true;
         }

          /*
               Jorge Garifuna - info@garidigital.com
               created: 9/12/06
               
               this function obtains the last inserted id of specified table 
          */
          function getMyLastId(&\$record){

               \$db = atkGetDb(); 
               // get the id of the last record that was inserted
               \$query = \"select max(\".\$this->primary_field.\") as last_id from \".\$this->table;
               \$rows = \$db->getrows(\$query);
               
               if(count(\$rows)>0){ 
                    \$id = \$rows[0][\"last_id\"];
                    \$record[\$this->primary_field] = \$id;
                    \$record[\"atkprimkey\"] = \$this->node.\".\".\$this->primary_field.\"='\$id'\";                                           
               }                       
          }
          /*
               This function is a wrapper to print messages to the top of the screen
               Author: Jorge Garifuna (GariDigital.com) <info@GariDigital.com>
               Created Date: 6/27/06

               @param \$message               The actual message to be printed
               @param \$background_color      The background color of the bar to show message
               @param \$text_color            The color of the text to be printed
               
               Note:
               - If your visual creativity is bad like mine, you can find nice colors at the following site:
                    http://www.colorcombos.com/index.html
                    
               - Make sure to include \"atkimport(\"atk.utils.atkmessagequeue\");\" at the top of your node 
          
          */
          function printMessage(\$message, \$background_color='#6787B0', \$text_color='white'){
               atkMessageQueue::addMessage(\"<div style='background-color: \".\$background_color.\";'><b><font color='\".\$text_color.\"'>\".\$message.\"</font></b></div>\"); //     FFAB35          
          }

          /* END NODE MESSAGING */               
               
               ";
               return $return_value;
          
          }

		function get_color_toggle_display($field_name, $is_statements){
			$return_value = "
		// obtains the color coded toggle display for $field_name
    		function rowColor(\$record){
               \$color = \"\";
$is_statements               
               return \$color;
          }// end rowColor function
			";			
                                                            /*
                                                            		// obtains the color coded toggle display for $field_name
                                                                		function ".$field_name."_display(\$record){
                                                                           \$color = \"\";
                                                            $is_statements               
                                                                           return \"<font color=\$color>\".\$record['".$field_name."'].\"</font>\";
                                                                      }// end function
                                                            */

			return $return_value;
		}// end function


		function get_view_page(){
			$return_value = "

		// this function is called when viewing an individual record
		// any information here will be displayed on the footer section of the viewed record page
		function viewPage(&\$handler, \$record, \$locked=false) 
		{ 
			return \$this->detailPageFooter(\$handler, \$record, \$locked,\"view\");
		}          
			";			

			return $return_value;
		}// end function

		
		function get_record_actions(){
			$return_value = "

          // including this function in the node, places a edit copy icon on the node so that 
          // the record edit screen is called right after the record has been copied
          function recordActions(\$record, &\$actions)  
          { 
                  \$actions[\"editcopy\"] = session_url(\"dispatch.php?atknodetype=\".\$this->atkNodeType().\"&atkaction=editcopy&atkselector=[pk]\", SESSION_NESTED); 
          }// end  function recordActions          

		// anything here will be processed prior to pre-copy insetion to database 
		function preCopy(&\$record){
               unset(\$record[\$this->primary_field]); //
		     return true;
		}// end function

			";			

			return $return_value;
		}// end function

		function get_edit_page(){
			$return_value = "

		// this function is called when editing an individual record
		// any information here will be displayed on the footer section of the edited record page
		function editPage(&\$handler, \$record, \$locked=false) 
		{ 
			return \$this->detailPageFooter(\$handler, \$record, \$locked);
		}          
			";			
			return $return_value;
		}// end function

		function get_page_footer(){
			$return_value = "

		
		// this is a custom helper function for editPage and viewPage
		// you can consolidate operations here unless you need to display different information
		// when editing and viewing the record
		// the information will be displayed on the footer page when editing or viewing an individual record	
		function detailPageFooter(&\$handler, \$record, \$locked=false, \$mode=\"edit\") 
		{ 
			if(\$mode == \"view\"){
				\$page = \$handler->viewPage(\$record, \$locked); 
			}else{// edit
				\$page = \$handler->editPage(\$record, \$locked); 
			}
			
			\$data_info = \"\";
			\$page .= \$data_info;
			
			return \$page; 
		}       // end function    

			";			
			return $return_value;
		
		}// end function


		function get_descriptor(){
			$return_value = "
		function descriptor_def(){
			return \$this->descriptor_field;
		}
			";			
			
			return $return_value;		

		}// end function

		function get_admin_header($node_name, $header_note){
               if(!empty($header_note)){ // remove comma's
                    $header_note = str_replace(chr(34),"\\".chr(34)."",$header_note);
               }
               
			$return_value = "

		// used for header section of list view page
		function adminHeader(){
               // get messages from the queue
               \$msgs =  atkMessageQueue::getMessages();
               if(count(\$msgs)>0){\$msgs = implode(\"<br>\", \$msgs);}
               else{\$msgs=\"\";}

		     \$help_text = \$msgs.\"
                    ".$header_note."
		          <!-- h1><font color=#FF6347>$node_name</font></h1 -->
		     \";
		     return \$help_text;
		}// end function
			";			
			
			return $return_value;
		
		}// end function

		function get_admin_footer($footer_note){
               if(!empty($footer_note)){ // remove comma's
                    $footer_note = str_replace(chr(34),"\\".chr(34)."",$footer_note);
               }

			$return_value_off = "
		// used for footer section of list view page
		function adminFooter(){
		     \$help_text = \"			                    
                         ".$footer_note."<br>
		          <b>Author:</b> - Jorge Garifuna (<a href='http://www.GariDigital.com' target=_blank>GariDigital.com</a>) <<a href='mailto:info@GariDigital.com'>info@GariDigital.com</a>> <br>
		          <b>Originally Created:</b> - ".date($this->date_format)." <br>
		          <b>Updated:</b> - ".date($this->date_format)." <br>
		          <b>Version:</b> - 1.0 <br>
		     \";
		     return \$help_text;
		}// end function
			";			
	
			$return_value = "
		// used for footer section of list view page
		function adminFooter(){
		     \$help_text = \"			                    
                         ".$footer_note."<br>
		          <h3>NusaSMS</h3>
		     \";
		     return \$help_text;
		}// end function
			";	

			return $return_value;		
		
		}// end function

          // obtains the goggle action/display functions for a field
		function get_toggle($field_name){
			$return_value = "

		/*
		What you need to do is create a custom action that can toggle the value. For example: 
		*/
		function action_".$field_name."() 
		{ 
		     \$id = \$this->m_postvars[\$this->primary_field]; 
		     \$recordset = \$this->selectDb(\$this->table.\".\".\$this->primary_field.\"=\".\$id); 
		     \$record = \$recordset[0]; // we only get back one record. 
		     // toggle the ".$field_name." value: 
		     \$record[\"".$field_name."\"] = abs(\$record[\"".$field_name."\"]-1); 
		     // Save back the value: 
		     \$this->updateDb(\$record); 
		
		     // Go back to the screen where we came from: 
		     \$this->redirect(); 
		}// end function 
		
		/*
		Finally, we have to hook the action somewhere. You can either create a toggle button in the action icons list, by defining a recordActions function, but, if as in your example, you want the value itself to be clickable, you can override its display method, like this: 
		*/
		function ".$field_name."_display(\$record, \$mode) 
		{ 
		     \$attr = &\$this->getAttribute(\"".$field_name."\"); 
		     \$original = \$attr->display(\$record, \$mode); 
		
		     // now wrap this in a clickable link, and pass the current record 
		     \$params[\$this->primary_field] = \$record[\$this->primary_field]; 
			return href(dispatch_url(\$this->module.\".\".\$this->node, \"".$field_name."\", \$params),\$original, SESSION_NESTED); 				                       
		}// end function 
			";			
			
			return $return_value;		
				  
		
		}// end function
		function get_initial_values(){
			$return_value = "

		// initializes values for adding records
		function initial_values(){             
		     \$init_vals = array();
		     \$init_vals[\"active\"] = 1;
			return \$init_vals;
		}// end function		
			";			
			
			return $return_value;		
		}// end funrion

          // added 11/16/06
          function get_descriptor_display($field, $mode){
               $return_value = "
		function ".$field."_display(\$record, \$mode) 
		{ 
		     \$attr = &\$this->getAttribute(\"".$field."\"); 
		     \$original = \$attr->display(\$record, \$mode); 

		     \$pparams[\$this->primary_field] = \$record[\$this->primary_field];  
		     \$pparams[\"atkselector\"] = \$this->node.\".\".\$this->primary_field.\"='\".\$record[\$this->primary_field].\"'\";
		     // link name
			\$original = href(dispatch_url(\$this->module.\".\".\$this->node, \"".$mode."\", \$pparams),\$original, SESSION_NESTED);
               return \$original; 				                       
          }// end function
			
			";			
			
			return $return_value;                
          }// end function

          // added 1/19/07
          // returns the master menu script caller function 
          function get_master_menu_load_script(){
               $return_value = "
          function load_my_scripts(\$mode=\"\"){
               \$page = &\$this->getPage();
               \$page->register_script(moduleDir(\"gktools\").\"js/pngfix.js\");
               \$page->register_style(moduleDir(\"gktools\").\"css/sliding_doors_tab_chrome.css\");
          
               \$js_code = \$this->get_js_code(\$mode);
               \$page->addContent(\$js_code);
               
          }// end function
			";						
			return $return_value;          
          }// end real function		

          // added 1/19/07
          // returns the master menu js script  function 
          function get_master_menu_js_code($menu_array, $menu_labels_array){
               $tabs = "\t\t\t\t\t\t";
               for($i=0; $i < count($menu_array); $i++){
                    $menu = $menu_array[$i];
                    $menu_label = $menu_labels_array[$i];
                    $params .= $tabs."\"".$menu."_title\"=>\"$menu_label\",\n";
                    $params .= $tabs."\"".$menu."_url\" => \$this->dispatch_url.\"?atknodetype=\".\$this->module.\".".$menu."&atkaction=admin&atklevel=1&atkprevlevel=0&source=".$menu."\",\n";
                    $params .= $tabs."\"".$menu."_image".$i."\"=>moduleDir(\"gktools\").\"images/blank.png\",\n\n";

                    $menu_items .= $tabs."<li '.((\$source==\"".$menu."\")?\"id=\\\"current\\\"\":\"\").'><a href=\"'.\$params[\"".$menu."_url\"].'\" title=\"\"><span><!-- img src=\"'.\$params[\"".$menu."_image".$i."\"].'\" border=0 align=absmiddle height=18 width=18 -->'.\$params[\"".$menu."_title\"].'</span></a></li>\n";

               }// end for
               
               $return_value = "
          function get_js_code(\$mode=\"\"){

               \$params = array();
               \$source = \$_REQUEST[\"source\"];

               \$params = array(                              
$params
                              );

               \$return_value = '
                         <div id=\"slidetabsmenu\">
                           <ul>
$menu_items
 
                           </ul>
                         </div>                         
                         <br style=\"clear: left;\" />
                         ';

               return \$return_value;
          }// end function
   		
			";						
			return $return_value;          
          }// end real function		

          // added 1/19/07
          // returns the master view function 
          function get_master_view($code){
               $return_value = "
          // called on record viewing
          function action_view(&\$handler){ 
               $code
               return \$handler->action_view();
          }
			";			
			
			return $return_value;          
          }// end real function		

          // added 1/19/07
          // returns the master add function 
          function get_master_add($code){
               $return_value = "
          // called on record adding
          function action_add(&\$handler){ 
               $code
               return \$handler->action_add();
          }
			";			
			
			return $return_value;          
          }// end real function		

          // added 1/19/07
          // returns the master edit function 
          function get_master_edit($code){
               $return_value = "
          // called on record editing
          function action_edit(&\$handler){ 
               $code
               return \$handler->action_edit();
          }
			";			
			
			return $return_value;          
          }// end real function		

          // added 12/22/06
          // returns the master admin function to control number of records shown on admin mode
          function get_master_admin($num, $code){
               $return_value = "
          function action_admin(&\$handler){ 
               global \$config_recordsperpage; 
               \$config_recordsperpage = $num;
               $code
               return \$handler->action_admin();
          }
			";			
			
			return $return_value;          
          }// end real function		

          
          // added 8/2/06
          // returns the master save function to remove AF_PRIMARY flag prior to saving to avoid problems with db_sequence table
          function get_master_save(){
               $return_value = "
          function action_save(&\$handler){
                  \$attr = &\$this->getAttribute(\$this->primary_field);
                  \$attr->removeFlag(AF_PRIMARY);
                  return \$handler->action_save();
          }// end function 
			";			
			
			return $return_value;          
          }// end function		

		function get_preadd(){
			$return_value = "

		// anything here will be processed prior to insetion to database 
		function preAdd(&\$record){
		     // \$record['created_date'] = date('Y-m-d');
               \$this->trackRecord(\"created\", \$record); 
		     return true;
		}// end function
			";
			
			return $return_value;
		}// end function 

		function get_preaddIntermediary($related_arra_values){
			$return_value = "

		// anything here will be processed prior to insetion to database 
		function preAdd(&\$record){
		     \$this->getSideNodesId(\$record);
               \$this->trackRecord(\"created\", \$record); 
		     return true;
		}// end function

          /*
               Jorge Garifuna - info@garidigital.com
               created: 9/12/06
               
               this function obtains the last inserted id of specified table 
          */
          function getSideNodesId(&\$record){
               \$db = atkGetDb();
               \$relations = array($related_arra_values);

               // traverse all m20 relations and get last id, if not existent
               foreach (\$relations as \$table_name => \$primary_field){
                    // check if field is empty and if it is retrieve last inserted id
                    if(empty(\$record[\$primary_field][\$primary_field])){
      
                         // get the id of the last record that was inserted
                         \$query = \"select max(\".\$primary_field.\") as last_id from \".\$table_name;
                         \$rows = \$db->getrows(\$query);
                        
                         if(count(\$rows)>0){ 
                              \$id = \$rows[0][\"last_id\"];
                              \$record[\$primary_field][\$primary_field] = \$id;
                         }    // end if                                              
                    }// end if               
               }// end foreach
               return true;
          }// end function


			";
			
			return $return_value;
		}// end function 


		function get_preupdate(){
			$return_value = "

		// anything here will be processed prior to updating record on db
		function preUpdate(&\$record) 
		{ 
		     // \$record['modified_date'] = date('Y-m-d'); 
		     \$this->trackRecord(\"modified\", \$record);
		     return true;
		}// end function
			";
			return $return_value;
		}// end function



		function get_trackrecord(){
			$return_value = "

          /*
               Jorge Garifuna
               11/16/06
               
               Obtains date, and user info to track record changes

               Assumes existance of attributes: 
                    - created_date {date or datetime}, created_by {int or varchar}, created_by_id {int}
                    - modified_date {date or datetime}, modified_by {int or varchar}, modified_by_id {int}
               
               
               @param \$prefix - may be \"created\" or \"modified\"
               @param \$record - the record to modify
               
               Call example:
                    \$this->trackRecord(\"created\", \$record); // to track record creation
                    
                    \$this->trackRecord(\"modified\", \$record); // to track record modification
          */
          function trackRecord(\$prefix, &\$record){ //
               if(empty(\$prefix)){return false;}
               \$a = \$this->getAttribute(\$prefix.\"_date\");               
		     if(is_object(\$a)){ // check object existance
                    if(\$a->dbFieldType() == \"datetime\"){
                         \$record[\$prefix.'_date'] = \$a->datetimeArray(); // datetime field //date('Y-m-d h:i:s');
                    }else if(\$a->dbFieldType() == \"date\"){ // date field
		             \$record[\$prefix.'_date'] = date('Y-m-d');                    
                    }                  
               }
               
               \$user = getUser();
               if(empty(\$user[\"user_id\"])){\$user[\"user_id\"]=0;}// if admin, set id to 0

               
               \$a = \$this->getAttribute(\$prefix.\"_by\");               
		     if(is_object(\$a)){
                    if(\$a->dbFieldType() == \"number\"){
                        \$record[\$prefix.'_by'] = \$user[\"user_id\"]; 
                    }else if(\$a->dbFieldType() == \"string\"){ 
		             \$record[\$prefix.'_by'] = \$user[\"name\"];                     
                    }                  
               }

               \$a = \$this->getAttribute(\$prefix.\"_by_id\");               
		     if(is_object(\$a)){
                    if(\$a->dbFieldType() == \"number\"){
                        \$record[\$prefix.'_by_id'] = \$user[\"user_id\"]; 
                    }else if(\$a->dbFieldType() == \"string\"){ 
		             \$record[\$prefix.'_by_id'] = \$user[\"name\"];                     
                    }                  
               }         
          }// end function

			";
			return $return_value;
		}// end function

		/* END INCLUSION OF OPTIONAL FUNCTIONS */


		function getAuthorNote($node, $type=""){
			$today = date($this->date_format);
			$label = "\tATK Node: $node\n\tTable: $node\n\tDate: $today\n\n";

 			if($type == "menu"){
			 	$label = "\tATK Module: $node\n\tDate: $today\n\n";
			}
			
			$return_value = "
/***********************************************************************
$label	
	This code was automatically generated by ATK Code Generator v".ACG_VERSION."
	
	@Author Jorge Garifuna (GariDigital.com) <info@GariDigital.com>
	
	Your are free to modify this code to fit your needs. 
	All I ask for is for your to keep this notice on top of the code
************************************************************************/		
			";
			
			return $return_value;
		}
		
		// obtains constructor enc code (5/28/06) 
		function getConstructorEnding($atabs, $ftabs, $include_table_controls=true){
               if($include_table_controls){
               	$return_value = $atabs."// index, order and table setups\n";
     		     $return_value .= $atabs."\$this->setIndex(\$this->index_field); // set index attribute\n";
     			$return_value .= $atabs."\$this->setOrder(\$this->order_field); // set order attribute(s)\n";
     			$return_value .= $atabs."\$this->setTable(\$this->table); // set node table\n\n";               
               }
			$return_value .= $ftabs."}// end constructor\n";
			
			return $return_value;
		}// end function				 


		// obtains the attribute relation code (5/27/06)
		function getAttributeRelationCode($properties_array, &$attribute_array){
			$node_type 		= $properties_array["node_type"];
			$relation_type 	= $properties_array["relation_type"];
			$name 			= $properties_array["attribute_name"];
			$source_table		= $properties_array["source_table"];
			$source_field 		= $properties_array["source_field"];
			$destination_table 	= $properties_array["destination_table"];
			$destination_field 	= $properties_array["destination_field"];
			$flags 			= $properties_array["attribute_flags"];
			$tab 			= $properties_array["attribute_tab"];
			$attribute_position	= $properties_array["attribute_position"];
			
			if(strlen($flags)<1){$flags=0;}  	// default flag to 0 if not existence
			else{$flags = str_replace("AF_PRIMARY|AF_HIDE |","",$flags); } // shave off AF_PRIMARY|AF_HIDE  from relation

			if(strlen($tab)>0){$tab=",".$tab.",".$attribute_position;} // add , before tab name of exists. This creates the second param on the add function
			else{$tab=",NULL,".$attribute_position;} // make tab null to add position
			
			if($node_type == "regular"){ // regular node
				if($relation_type == $this->relation_type_array["o2m"]){ // o2m
					if(strstr($flags,"AF_OBLIGATORY")){ // o2m
						$flags = str_replace("AF_OBLIGATORY","0",$flags); // remove obligatory flag from o2m  
					}
					
					
					if($flags == "0"){
						$flags = "AF_HIDE_LIST | AF_CASCADE_DELETE"; // hide relation from list and cascade delete
					}else{
						$flags .= "| AF_HIDE_LIST | AF_CASCADE_DELETE"; // hide relation from list and cascade delete
					}

					if(strstr($flags,"0 | AF_ONETOMANY_EMBED")){ // o2m
						$flags = str_replace("0 | AF_ONETOMANY_EMBED","AF_ONETOMANY_EMBED",$flags); // remove 0, from flags if it contains AF_ONETOMANY_EMBED on o2m  
					}

					$attribute_array[] = "\$this->add(new atkOneToManyRelation(\"".$destination_table."\",\$this->module.\".".$destination_table."\",\"".$destination_field."\", ".$flags.")".$tab.");";
				}else if($relation_type == $this->relation_type_array["m2o"]){ // m2o
                         $this->function_includes_array["preAddIntermediary"]["values"] .= "'".$destination_table."'=>'".$source_field."',";
					$attribute_array[] = "\$this->add(new atkManyToOneRelation(\"".$source_field ."\",\$this->module.\".".$destination_table."\", $flags)".$tab.");";
				}else if($relation_type == $this->relation_type_array["shuffle"]){ // shuffle

					if(strstr($flags,"AF_OBLIGATORY")){ // shuffle
						$flags = str_replace("AF_OBLIGATORY","0",$flags); // remove obligatory flag from shuffle  
					}

					$attribute_array[] = "\$this->add(new atkShuttleRelation(\"".$destination_field."\",\$this->module.\".".$destination_table."\",\$this->module.\".".$destination_field."\", $flags)".$tab.");";
				}else if($relation_type == $this->relation_type_array["bool"]){ // bool
					if(strstr($flags,"AF_OBLIGATORY")){ // bool
						$flags = str_replace("AF_OBLIGATORY","0",$flags); // remove obligatory flag from bool relation  
					}

					$attribute_array[] = "\$this->add(new atkManyBoolRelation(\"".$destination_field."\",\$this->module.\".".$destination_table."\",\$this->module.\".".$destination_field."\", $flags)".$tab.");";
				}// end if			
			}else if($node_type == "meta"){ // meta node
				if($flags != "0"){ // set flags for meta field
					if($relation_type == "o2m" && strstr($flags,"AF_OBLIGATORY")){ // o2m
						$flags = str_replace("AF_OBLIGATORY","0",$flags); // remove obligatory flag from o2m
					}
					$attribute_array[] = "\$policy->setFlags(\"".$name."\",".$flags.");";
				}			

				if($relation_type == "o2m"){ // o2m					
					$attribute_array[] = "\$policy->hasMany(\"".$destination_table."\", array(\"dest\" => \"".$destination_field."\")); ";
				}else if($relation_type == "m2o"){ // m2o				 
					$attribute_array[] = "\$policy->hasOne(\"".$destination_table."\", array(\"source\" => \"".$source_field."\"));";
				}
				
			}// end if
		}// end function
		
         
		// obtains the attribute code (5/27/06)
		function getAttributeCode($properties_array, &$attribute_array, &$create_folder,&$attribute_relation_array, &$node_include_array, $attributes_count){
			$type 			= $properties_array["attribute_type"];
			$db_type 			= $properties_array["attribute_db_type"];
			$name 			= $properties_array["attribute_name"];
			$flags 			= $properties_array["attribute_flags"];
			$tab 			= $properties_array["attribute_tab"];
			$module 			= $properties_array["module"];
			$node_name		= $properties_array["node_name"];
			$options 			= $properties_array["attribute_options_array"];
			$force_update_array = $properties_array["force_update_array"];
			$attribute_position	= $properties_array["attribute_position"];
			
			$name_prefix = ""; // used if field is force update
			$force_update_code = "";
			
			if((array_key_exists($name,$force_update_array)) && ($force_update_array[$name])){
				$name_prefix = "\$".$name." = &";
				$force_update_code = "\$".$name."->setForceUpdate(true); // for field to be updated on database after edit function";
			}
			
			// flags management
			if(strlen($flags)<1){$flags=0;}  	// default flag to 0 if not existence
			else if((strstr($flags,"AF_PRIMARY|AF_HIDE")) && ((strstr($flags,"| AF_OBLIGATORY")) || (strstr($flags,"| AF_ONETOMANY_EMBED"))) ){
                    // shave off | AF_OBLIGATORY from primary key
                    $flags = str_replace("| AF_OBLIGATORY","",$flags);

                    // shave off | AF_ONETOMANY_EMBED from primary key. added 12/30/06
                    $flags = str_replace("| AF_ONETOMANY_EMBED","",$flags);
   
               } 

               // tabs management
			if(strlen($tab)>0){$tab=",".$tab.",".$attribute_position;} // add , before tab name of exists. This creates the second param on the add function
			else{$tab=",NULL,".$attribute_position;} // make tab null to add position
			
			// attributes management
               if(($this->parent_fields_array["state"]) && ($this->parent_fields_array["field"][$name]["state"]) && (array_key_exists($name,$this->parent_fields_array["field"]))){
                    if($flags == 0 && strlen($this->parent_fields_array["field"][$name]["flags"])>0){
                         $flags = $this->parent_fields_array["field"][$name]["flags"];
                    }else if($flags != 0 && strlen($this->parent_fields_array["field"][$name]["flags"])>0){
                        $flags .= " | ".$this->parent_fields_array["field"][$name]["flags"];
                    }
                    $attribute_array[] = $name_prefix."\$this->add(new atkManyToOneTreeRelation(\"".$name."\",\$this->module.\".".$node_name."\", ".$flags.")".$tab.");";
                    
                    $attributes_count = ($attributes_count * 100 + 10);
                    // make child items of this parent show as recordlist when parent detail is viewed
                    $attribute_relation_array[]  = "\$this->add(new atkOneToManyRelation(\"child_items\",\$this->module.\".".$node_name."\",\"".$name."\", AF_HIDE_LIST | AF_CASCADE_DELETE), NULL,".$attributes_count.");";
                    
                    // include the o2m header for child of parent
                    $this->getNodeIncludes(array("relation_type"=>$this->relation_type_array["o2m"]),$node_include_array); // fill node includes
                    
               }else if($type == $this->attributes["currency"]){ // currency
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", ".$flags.",15, \"\$\", 2,\".\",\",\")".$tab.");";
			}else if($type == $this->attributes["number"]){ // number, modified 12/22/06
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", ".$flags.",10,0)".$tab.");";
			}else if($type == $this->attributes["duration"]){ // duration, added 12/22/06
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\",\"1m\",\"10h\", ".$flags.",0)".$tab.");";
			}else if($type == $this->attributes["time"]){ // time, added 12/22/06
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\",0,23,array(\"00\",\"15\",\"30\",\"45\"),\"\", ".$flags.")".$tab.");";
			}else if($type == $this->attributes["country"]){ // country, added 12/30/06
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\",\"world\",\"\",\"\", ".$flags.",true)".$tab.");";
			}else if($type == $this->attributes["state"]){ // state, added 12/30/06
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\",\"usa\",\"\",\"\", ".$flags.",true)".$tab.");";
			}else if($type == $this->attributes["file"]){ // files
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", moduleDir(\"".$module."\").\"".$this->attachment_files_folder."\",  ".$flags.")".$tab.");";
				$create_folder = true; // signal creation of folder for this node
				$this->attachment_files_detected = true;
			}else if($type == $this->attributes["date"]){ // date
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", \"".$this->date_format."\",\"".$this->date_format."\",0,0, ".$flags.")".$tab.");";			
			}else if($type == $this->attributes["bool"]){ // bool
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", ".$flags.")".$tab.");";				
			}else if($type == $this->attributes["email"]){ // email
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", false, ".$flags.")".$tab.");";				
			}else if($type == $this->attributes["password"]){ // password
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", false, ".$flags.")".$tab.");";				
			}else if($type == $this->attributes["color"]){ // color
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", \"\",".$flags.")".$tab.");";				
			}else if($type == $this->attributes["radio"]){ // radio
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", array(".$db_type."), array(".$db_type."), ".$flags.")".$tab.");";				
			}else if($type == $this->attributes["list"]){ // list
                    $db_type = str_replace("enum(","",$db_type); // remove enum from db value
                    $db_type = str_replace("')","'",$db_type); // remove ') from db value 
                    
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", array(".$db_type."), array(".$db_type."), ".$flags.")".$tab.");";				
			}else if($type == $this->attributes["multilist"]){ // multi list
				$attribute_array[] = "// db type SET is delimeted by comman (,)";
				$attribute_array[] = "\$msa = new ".$type."(\"".$name."\", array(".$db_type."), array(".$db_type."), \"\",".$flags.");";
				$attribute_array[] = "\$msa->setDelimeter(\",\");";
				$attribute_array[] = $name_prefix."\$this->add(\$msa".$tab.");";
				//$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", array(".$db_type."), array(".$db_type."), \"\",".$flags.")".$tab.");";				
			}else if($type == $this->attributes["text"]){ // text
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", \"\",".$flags.")".$tab.");";				
			}else if($type == $this->attributes["mce"]){ // mce
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", array(700,400),".$flags.")".$tab.");";				
			}else if($type == $this->attributes["fck"]){ // fck
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", array(\"rows\"=>6,\"cols\"=>6),".$flags.",array('ToolbarSet'=>'".$this->fck_toolbar_type."','Width'=>'450px', 'Height'=>'200px'))".$tab.");";				
                    $this->fck_attributes_array[$name] = $name;
			}else{ // regular attribute
				$attribute_array[] = $name_prefix."\$this->add(new ".$type."(\"".$name."\", ".$flags.")".$tab.");";			
			}
			
			if(strlen($force_update_code)>0){ // set for update, if applicable
				$attribute_array[] = $force_update_code;
			}
		}// end function
		
		// obtains the variable name of the tab (5/27/06)
		function getTabVariable($tab_name, &$tab_array){
			$tab_variable = "";
			if(strlen($tab_name)>0){			
				$tab_variable = strtolower("\$".str_replace(" ","_", $tab_name)."_tab_label");
				if(!array_key_exists($tab_variable,$tab_array)){ // only add variable if not existance
					$tab_array[$tab_variable]=$tab_name;
				}				
			}			
			return $tab_variable; 
		}// end function

         // obtains the include sections of the nodes (5/27/06)
	    function getNodeIncludes($node_options_array, &$include_array){ // $node_type: normal, meta, tree
			$node_type = $node_options_array["node_type"];
			$attribute_type = $node_options_array["attribute_type"];
			$relation_type = $node_options_array["relation_type"];

	    		$at = strtolower($attribute_type);
	    		
			if(($node_type == "meta") && !array_key_exists("atk.atkmetanode",$include_array)){
			    $include_array["atk.atkmetanode"] = "atkimport(\"atk.atkmetanode\");";
			}else if(($relation_type == $this->relation_type_array["m2o"]) && (!array_key_exists("atkmanytoonerelation",$include_array))){			
				$include_array["atkmanytoonerelation"] = "userelation(\"atkmanytoonerelation\");";
			}else if(($relation_type == $this->relation_type_array["o2m"]) && (!array_key_exists("atkonetomanyrelation",$include_array))){			
				$include_array["atkonetomanyrelation"] = "userelation(\"atkonetomanyrelation\");";
			}else if(($relation_type == $this->relation_type_array["shuffle"]) && (!array_key_exists("atkshuttlerelation",$include_array))){			
				$include_array["atkshuttlerelation"] = "userelation(\"atkshuttlerelation\");";
			}else if(($relation_type == $this->relation_type_array["bool"]) && (!array_key_exists("atkmanyboolrelation",$include_array))){			
				$include_array["atkmanyboolrelation"] = "userelation(\"atkmanyboolrelation\");";
			}// end if

			if((strlen($at)>0) && ($at != $this->attributes["attribute"]) && (!array_key_exists($at,$include_array))){
			    $mce_module = "";
			    if($at == strtolower($this->attributes["mce"])){
			    		$mce_module = $this->mce_module.".";
			    }
			    $include_array[$at] = "useattrib(\"".$mce_module.$at."\");";				
			}
			
			// include parent id relation if any
               if($this->parent_fields_array["state"]){
                    $include_array["atk.atktreenode"] = "atkimport(\"atk.atktreenode\");\nuserelation(\"atkmanytoonetreerelation\"); ";
               }

               // include the messaging: added June 27, 2006 
			if($this->function_includes_array["getMessage"]["state"]){
				$include_array["atk.utils.atkmessagequeue"] = "atkimport(\"atk.utils.atkmessagequeue\");";
			}


	    }// end function
	     
          // finds the relationships between selected tables
          // entry function to create relationships
          function find_relations($data){
              for($i=0;$i<count($data); $i++) {
                    $table_name = $data[$i];
                    $this->fill_relation($table_name, $data);
              }// end for

			// normalize relations to include those m2o that were excluded 	
	 	    $this->normalize_relations();
	 	    
          }// end function
          
          // creates the relationships of a node
          function fill_relation($source_table, $all_tables){
               $db = atkGetDb();               
               $query = "SHOW KEYS FROM ". $source_table;          
               $db->query($query);
               $source_field = "";

               // find primary key of provide table
			while ($db->next_record())
               {
     			$key_name = $db->m_record["Key_name"];
     			$column_name = $db->m_record["Column_name"];
     			if(strtolower($key_name)=="primary"){ // capture source primary field
                         $source_field = $column_name;
                    }
               }// end while 

			//print "<font color=red>$source_table: $source_field</font><br>";
               if(!empty($source_field)){ // only proceed if source has a primary key
               
                   for($i=0;$i<count($all_tables); $i++) {
                         $table_name = $all_tables[$i];
                         if($source_table != $table_name){ // exclude provide table
                              //$query = "SHOW KEYS FROM ". $table_name;          
                              $query = "SHOW FIELDS FROM ". $table_name; // get all fields from target table
                              $db->query($query);
                              while ($db->next_record()){
   			     			 $key_name = $db->m_record["Key"]; //$db->m_record["Key_name"];
     			                $column_name = $db->m_record["Field"]; //$db->m_record["Column_name"];

     			                // we've found our destination field, that is not primary key of the destination table
                                    if(($column_name == $source_field) && (strtolower($key_name)!="pri") ){

                                        // source table already have a relation, just append destination
                                        if(array_key_exists($source_table, $this->relations_array)){
                                             $this->relations_array[$source_table]->addDestination($table_name, $column_name, $source_field, $this->relation_type_array["o2m"]);
                                        }else{ // new relation for source table
                                             $this->relations_array[$source_table] = new relation_source($source_table, $source_field);
                                             $this->relations_array[$source_table]->addDestination($table_name, $column_name, $source_field, $this->relation_type_array["o2m"]);                                             

                                             // we need to reciprocate relationship. check existence of destination table
                                             if(array_key_exists($table_name, $this->relations_array)){
                                                  $this->relations_array[$table_name]->addDestination($source_table, $source_field, $column_name, $this->relation_type_array["m2o"]);
                                             }else{ // new relation to destination 
                                                  $this->relations_array[$table_name] = new relation_source($table_name, $source_field);
                                                  $this->relations_array[$table_name]->addDestination($source_table, $source_field, $column_name, $this->relation_type_array["m2o"]);                                             
                                             }// end if(array_key_exists($table_name, $this->relations_array)){
                                        }// end if(array_key_exists($source_table, $this->relations_array)){
                                    }// end if(($column_name == $source_field) && (strtolower($key_name)!="pri") ){
                              }// end while                               
                         }// end if($source_table != $table_name){
                   }// end for               				
               }   // end if   if(!empty($source_field)){         
          }// end function
          

          // checks to make sure that all relationships are covered on both ends: o2m and m2o
          // and perform adjustments if necessary
          function normalize_relations(){
               $relations_count = count($this->relations_array);
               
				 foreach ($this->relations_array as $source_relation) 
                     { 
                         $table_name = $source_relation->getTableName();
                         //print $table_name."<br>";
					$source_field = $source_relation->getPrimaryKey();
                         
                         // get destination tables
                         while($relation_data = $source_relation->getNextDestination()){
                              $destination_table = $relation_data->getDestinationTable();
                              $destination_field = $relation_data->getDestinationField();
						$relation_type = $relation_data->getRelationType();                                                         

                              $related_back = false;                              
                              // does this table relate to me?
                              // check if already included on global variable
                              if(array_key_exists($destination_table, $this->relations_array)){
                                   $destination_relation = $this->relations_array[$destination_table];

                                   while($destination_data = $destination_relation->getNextDestination()){
                                        $destination_table_source = $destination_data->getDestinationTable();
                                        if($destination_table_source == $destination_table){
                                             $related_back = true;     
                                        }// end if
                                        
                                   }// end while                                   
                              }// end if
						
						if(!$related_back){// destination not related back to source
							if($relation_type == $this->relation_type_array["m2o"]){
								$relation_type =$this->relation_type_array["o2m"];
							}else if($relation_type == $this->relation_type_array["o2m"]){
								$relation_type =$this->relation_type_array["m2o"];
							}
							if(!array_key_exists($destination_table, $this->relations_array)){
								$this->relations_array[$destination_table] = new relation_source($destination_table, $source_field);
							}
							$this->relations_array[$destination_table]->addDestination($table_name, $destination_field, $destination_field, $relation_type);
                              }  // end if                            
                         }// end while                          
                     } // end foreach
                     //print "<hr>";
          }// end function
         
          // obtains the relationships of a node
          function get_relations($table_name, $rowspan=1, &$relationships_array){
               $return_value="<td align=left valign=top rowspan='".$rowspan."'>";
               if(array_key_exists($table_name, $this->relations_array)){
                    $relation_destination = $this->relations_array[$table_name];
                    while($relation_data = $relation_destination->getNextDestination()){
                         $destination_table = $relation_data->getDestinationTable();
        				$destination_field = $relation_data->getDestinationField();
        				$source_field =  $relation_data->getSourceField();

                         $relation_type = $relation_data->getRelationType();

                         $other_relations = "";
                         // check if we have a o2m relation, so we can recommend a shuffle/manybool relation, if applicable
                         // a shuffle/manybool relation is constructed if the o2m relation has a m2o relation that is not of the subject node
                         // if the o2m relation has many m2o relations, then we show all of them in a list
                         if($relation_type==$this->relation_type_array["o2m"]){
                              
                              $other_relations = $this->get_other_relations($destination_table, $source_field, $table_name);
                              
                              if(!empty($other_relations)){ // we have options for shuffle/many bool relations
                                   $note = "<b>note:</b> To enabled these relations uncheck the o2m relation from the left. As soon as a checkbox becomes unchecked all checkboxes become enabled. You can only select one relationship at the time.";
                                   $other_relations = "<td>".$this->get_content_div($other_relations.$note)."<td>\n"; 
                              }
                              
                         }                                   
                         
                         $js_call=""; // only fill up if relation is o2m
                         if($relation_type==$this->relation_type_array["o2m"]){
                              $js_call = "onClick='manageRelationCheckboxes(this);'";
                         }
                         $checkbox = "&nbsp;<input  ".$js_call." type='checkbox' value='".$table_name."*".$source_field."*".$destination_table."*".$destination_field."*".$relation_type."' name='".$table_name."*".$source_field."_relation[]' checked>";
                         // replace _ with <br> for relation display name
                         $return_value .= "<table><tr>
                                             <td align=left>".$checkbox."</td>
                                             <td align=left>".str_replace("_","<br>",$destination_table)."<br>(".$relation_type.")</td>
                                             ".$other_relations."
                                          </tr></table>";
					// log m2o relations so that we can automatically mark the AF_RELATION_AUTOLINK checkbox
					if((($relation_type==$this->relation_type_array["m2o"]) || ($relation_type==$this->relation_type_array["o2m"])) && (!array_key_exists($source_field, $relationships_array))){
						$relationships_array[$source_field] = $relation_type;
					}
					
                    }
                    //$this->relations_array[$table_name]->addDestination($source_table, $source_field, $column_name, $this->relation_type_array["m2o"]);                                             
               }
               
               $return_value .= "&nbsp;</td>";
               return $return_value;
          }

          // obtains the other relationships of a node, to suggest shuffle/many bol relations
          function get_other_relations($table_name, $subject_field, $subject_table){
               $return_value="";
               if(array_key_exists($table_name, $this->relations_array)){

                    $other_relations_array   = array(); // array for all m2o relations so we can place them on a list later
                    $relation_destination    = $this->relations_array[$table_name];
                    while($relation_data     = $relation_destination->getNextDestination()){
                         $destination_table  = $relation_data->getDestinationTable();
        				$destination_field  = $relation_data->getDestinationField();
        				$source_field       =  $relation_data->getSourceField();

                         $relation_type      = $relation_data->getRelationType();


                         $other_relations    = "";
                         
                         // check if we have a o2m relation, so we can recommend a shuffle/manybool relation, if applicable
                         // a shuffle/manybool relation is constructed if the o2m relation has a m2o relation that is not of the subject node
                         // if the o2m relation has many m2o relations, then we show all of them in a list
                         if(($relation_type==$this->relation_type_array["m2o"]) && ($destination_table != $subject_table)){
                              $other_relations_array[] = "<option value='".$destination_table."'>".$destination_table."</option>";                             
                         }// end if
					
                    }// end while

                    // we have m2o relations, now construct the list to show the shuffe/many bool relation recommendations
                    if(count($other_relations_array)>0){
                         $checkbox_shuffle = "&nbsp;<input  onClick='manageRelationCheckboxes(this);' type='checkbox' value='".$subject_table."*".$subject_field."*".$table_name."*".$subject_field."*".$this->relation_type_array["shuffle"]."' name='".$subject_table."*".$subject_field."_relation[]' disabled>";
                         $checkbox_bool = "&nbsp;<input  onClick='manageRelationCheckboxes(this);' type='checkbox' value='".$subject_table."*".$subject_field."*".$table_name."*".$subject_field."*".$this->relation_type_array["bool"]."' name='".$subject_table."*".$subject_field."_relation[]'  disabled>";
                         $return_value = $this->get_other_relation_format(
                                                                      "<b>Shuffle Relation:</b>",
                                                                      $this->relation_type_array["shuffle"],
                                                                      $checkbox_shuffle,
                                                                      $table_name,
                                                                      $subject_table,
                                                                      $subject_field,
                                                                      $other_relations_array);                                                                      

                         $return_value .= $this->get_other_relation_format(
                                                                      "<b>Many Bool Relation:</b>",
                                                                      $this->relation_type_array["bool"],
                                                                      $checkbox_bool,
                                                                      $table_name,
                                                                      $subject_table,
                                                                      $subject_field,
                                                                      $other_relations_array);                                                                      
                    }// end if(count($other_relations_array)>0){
               }// end if(array_key_exists($table_name, $this->relations_array)){
               
               return $return_value;
          }
          
          // obtains the format of the other relation HTML table 
          function get_other_relation_format($label, $relation_type, $checkbox, $source_table, $subject_table, $subject_field,$other_relations_array){
               $relation_list = "";
               if(count($other_relations_array)>0){
                    $relation_list = "<!-- start other relation list -->\n<select name='".$subject_table."*".$subject_field."_".$relation_type."_relation'>\n".implode("\n",$other_relations_array)."\n</select>\n<!-- end other relation list -->\n";
               }// end if

               $return_value = "
                              <!-- start $relation_type other relation for $subject_table table -->
                              <table>
                                   <tr>
                                   <td>".$label."</td>
                                   <td>".$checkbox."</td>
                                   <td>".str_replace("_","<br>",$source_table)."</td>
                                   <td>".$relation_list."</td>
                                   </tr>
                              </table>
                              <!-- end $relation_type other relation for $subject_table table -->
                         ";

               return $return_value;
          }// end function

          // obtains the hidden for content. Full content will display when title clicked and hide when clicked again
          function get_content_div($content, $label="other", $header_style="", $content_style=""){
               $return_value="<div class=\"dhtmlgoodies_question\" ".$header_style.">".$label."</div>
                    <div class=\"dhtmlgoodies_answer\" ".$content_style.">
                    	<div>
                              <!-- start content -->
                              ".$content."
                              <!-- end content -->
                    	</div>
                    </div>
               ";
               return $return_value;
          
          }

          // nodes detail options and module setup prior to code generation
          function preview_form($data){
               if(!is_array($data)){return;}

                $ui = &$this->getUi();
                $this->addStyle("style.css");
                $this->addStyle("recordlist.css");
                
                
                if (is_object($ui))
                {
                    $page = &$this->getPage();
                    $page->head(atktext("dashboard"));
                    $page->body();
                    //$page->register_script(moduleDir("webstoredl")."js/test.js");
                    $page->register_script("./atk/javascript/tools.js");
                    $page->register_script("./atk/javascript/recordlist.js");
                    $page->register_script("./atk/javascript/formselect.js");
                    $page->register_script("./atk/javascript/class.atkwysiwygattribute.js");

                    // add languages. 1/5/07
                    $lcn = 0;
                    $lcols = 10;
                    $languages = "";
                    foreach($this->supported_languages as $code => $value){
                         $label = $value["label"];
                         $selected = ($value["selected"])?"checked":"";
                         $languages .= "<input type='checkbox' name='languages[]' ".$selected." value='".$code."'>".$label."&nbsp;&nbsp;";
                         $lcn++;
                         if($lcn == $lcols){
                              $languages .= "<hr>";
                         }
                    }// end for
                    /*
                    // show languages
                    if(!empty($languages)){
                          $content .= "<tr><td colspan=2><b>Supported Languages:</b><br>$languages<hr></tr>";
                    }// end if $languages
                    */

                    // automatically select globla navigation if there are more than one nodes
                    // added 1/19/07
                    if(count($data)>1){$global_nav = "checked";}
                    else{$global_nav = "";}   
                                     
                    $content = 
					$this->getJSCustom().				
                         "<table width=100% class='recordlist' cellspacing=0> 
                              <form  name='".$this->form_name."' method=post action='".$this->dispatch_url."'  onSubmit='return enableCheckboxes()' onKeyUp='highlight(event)' onClick='highlight(event)'>
                                <input type='hidden' name='atkprevlevel' value='0'>
                                <input type='hidden' name='atkstackid' value='445670af19b1b'>
                                <input type='hidden' name='atkapp' value='99b165f709d98684780c9bc0596c237d'>
                                <input type='hidden' name='atkescape' value=''>
                                <input type='hidden' name='atknodetype' value='".$this->module.".".$this->node."'>
                                <input type='hidden' name='atkaction' value='".$this->form_action["finish"]."'>
                                <script language='javascript' type='text/javascript'>var ".$this->form_name." = new Object();</script>

						<!-- start module -->
				            <tr>					      
					      <td align='left' colspan=2 >
						 
						 	<table width='100%' border='0' cellspacing='0' cellpadding='0'>
							  <tr>
							      <th valign='middle' width='150'>Module Name</th>      
								  <th align='right' valign='middle' >Backup Files</th>
								  <th align='right' valign='middle' >Create Global Navigation <a href='javascript:alert(\"Global navigation sets a standard set of menu across every node.\\nFor this work you need a module called gktools with subfiles: js/pngfix.js and css/sliding_doors_tab_chrome.css.\\nSliding doors tab menu can be downloaded from: \\n\\thttp://www.dynamicdrive.com/style/csslibrary/item/sliding-doors-tabs-menu/\")'>?</a></th>
                                          <th align='right' valign='middle' >Supported Languages</th>      
								</tr>
							    
							    <td><input type=text name='module_name' size=60></td>
							    <td align=right><input type='checkbox' name='backup_files' checked></td>
                                        <td align=right><input type='checkbox' name='global_navigation' $global_nav></td>
							    <td align=right>$languages</td>
							  </tr>
							</table>

						 
						 </td>
				            </tr>
							<!-- end module -->
							
                         \n";
                    

                    $content .= "
                              <!-- start table body  added 1/19/07 -->
                              <tbody style='height:390px; overflow: auto;'>
                    ";
                         
                    for($i=0;$i<count($data); $i++){
                         $table_name = $data[$i];
                         $tr_row = (($i%2)==0)?2:1;
                         $content .= "<tr
                                             id=\"".$this->form_name."_".$i."\" 
                                              class=\"row".$tr_row."\"                    
                                              onmouseover=\"highlightrow(this, '#999999')\"
                                             onmouseout=\"resetrow(this)\"
                                             onclick=\"selectrow(this, '".$this->form_name."', ".$i.")\"
                         
                                        ><td align=left width=5>".($i + 1)."
								<td align=left>
                                              ".$this->get_node_format($table_name)."<br>
                          				".$this->get_attribute_format($table_name)."
									<hr>

                                             </td></tr>\n";
                    }// end for

                    $content .= "
                              </tbody>
                              <!-- end table body  -->
                              
				            <!-- start footer -->
				            <tr>					      
					      <th valign='middle' > &nbsp;</th>
					      <th valign='middle' > &nbsp;</th>
				            </tr>
							<!-- end footer -->

                        <!-- multirecord actions -->
                       <tr>
                         <td align='left'><td align='left'>
                           <table border='0' cellspacing='0' cellpadding='2'>
                             <tr>
                               <td valign='top'></td>
                               <td>
                                 <!-- a href=\"javascript:updateSelection('".$this->form_name."', document.forms['".$this->form_name."'], 'all')\">Select all</a> / <a href=\"javascript:updateSelection('".$this->form_name."', document.forms['".$this->form_name."'], 'none')\">Deselect all</a> / <a href=\"javascript:updateSelection('".$this->form_name."', document.forms['".$this->form_name."'], 'invert')\">Invert selection</a>&nbsp; <input type='hidden' name='".$this->form_name."_atkaction' value='".$this->form_action["finish"]."'><input type='button' class='btn btn-primary' value='".$this->form_action["finish"]."' onclick=\"atkSubmitMRA('".$this->form_name."', this.form, '".$this->dispatch_url."')\" --> 
							&nbsp; <input type='hidden' name='".$this->form_name."_atkaction' value='".$this->form_action["finish"]."'><input type='submit' class='btn btn-primary' value='".$this->form_action["finish"]."'>
                               </td>
                             </tr>
                           </table>
                         </td>
                       </tr>
                      </table><br></td>
                      </tr>
                    \n";
   
                    $content .= "</form>\n</table>\n";

                    $version_info = $this->get_version();
                    $content .= $version_info; 
                    
                    $content_box = $ui->renderBox(array("title"=>atktext("Selected tables for code generation of database ".$this->db_name),
                                                         "content"=>$content)
                                                         
                                                         );

                  $page->addContent($content_box);

               }// end if (is_object($ui))
			
		}// end function

		// obtains node flag options for disply
          function get_node_format($node_name){
			$total_node_flags = count($this->node_flags_array);
			$menu_label = str_replace("_"," ",$node_name);
			$menu_label = ucwords($menu_label);
			$return_data = "		
				<input type=hidden name='node_name[]' value='$node_name'>
				<table border='1' cellspacing='0' cellpadding='0' class='recordlist'>
				  <tr>
				    <td colspan='".($total_node_flags+8)."' ><h3><font color='#CE0000'><b>$node_name</b></font> Node</h3></td>
				    <td align='right'><b>Menu Label:</b><input type='text' name='".$node_name."_node_menu_label' value='$menu_label'></td>
				  </tr>
				  <tr>                    
                        <th align=center style='font-size: ".$this->header_font_size."'>Records<br>per<br>Page</th> 
					";
				$checkboxes = "
                              <td>
                              <select name='".$node_name."_node_records_row'>
                                   <option>none</option>
                                   <option>10</option>
                                   <option>25</option>
                                   <option selected>50</option>
                                   <option>100</option>
                                   <option>125</option>
                                   <option>150</option>
                                   <option>200</option>
                                   <option>250</option>
                                   <option>300</option>
                              </select>
                              </td>";
				for($i=0; $i < $total_node_flags; $i++){
					$name = $this->node_flags_array[$i]->getFlagName();
					$label = $this->node_flags_array[$i]->getLabel();
					$value = $this->node_flags_array[$i]->getValue();
					$type = $this->node_flags_array[$i]->getType();
					$checked = ($value==1)?"checked":"";

                         // substitute spaces on label with <br>
					$return_data .= "					  

					    <th align=center style='font-size: ".$this->header_font_size."'>".str_replace(" ","<br>",$label)."</th>				  

				  	";
					$checkboxes .=" <td align=center >&nbsp;<input type='$type' value='$name' name='".$node_name."_node_options[]' $checked></td>\n";

				  }// end for

                    // smart search
				$checkboxes .=" <td align=center >&nbsp;<input type='checkbox' value='$node_name' name='".$node_name."_node_smart_search' checked></td>\n";
				
                    // event log
				$checkboxes .=" <td align=center >&nbsp;<input type='checkbox' value='$node_name' name='".$node_name."_node_event_log' checked></td>\n";

                    // row counter				
                    $checkboxes .=" <td align=center >&nbsp;<input type='checkbox' value='$node_name' name='".$node_name."_node_counter' checked></td>\n";

				// include menu
                    $checkboxes .=" <td align=center >&nbsp;<input type='checkbox' value='$node_name' name='".$node_name."_node_include_menu' checked></td>\n";

				// include messaging
                    $checkboxes .=" <td align=center >&nbsp;<input type='checkbox' value='$node_name' name='".$node_name."_node_include_messaging' checked></td>\n";

				// include global search
                    $checkboxes .=" <td align=center >&nbsp;<input type='checkbox' value='$node_name' name='".$node_name."_node_include_global_search' checked></td>\n";

				// node descriptor_link
				$checkboxes .=" <td align=left nowrap>
								&nbsp;<input type='radio' value='' name='".$node_name."_node_descriptor_link'>None<br>
								&nbsp;<input type='radio' value='view' name='".$node_name."_node_descriptor_link'  checked>View<br>
								&nbsp;<input type='radio' value='edit' name='".$node_name."_node_descriptor_link'>Edit<br>
				
							</td>";		
                    
				// node type
				$checkboxes .=" <td align=left nowrap>
								&nbsp;<input type='radio' value='regular' name='".$node_name."_node_type' checked> Regular<br>
								&nbsp;<input type='radio' value='meta' name='".$node_name."_node_type' > Meta<br>
								&nbsp;<input type='radio' value='tree' name='".$node_name."_node_type' disabled> Tree<br>
				
							</td>";		
				
				$return_data .= "
                              <th align=center style='font-size: ".$this->header_font_size."'>Smart<br>Search</th>
                              <th align=center style='font-size: ".$this->header_font_size."'>Event<br>Logs</th>
                              <th align=center style='font-size: ".$this->header_font_size."'>Row<br>Counter</th>
						<th align=center style='font-size: ".$this->header_font_size."'>Include<br>Menu</th>
                              <th align=center style='font-size: ".$this->header_font_size."'>Include<br>Messaging</th>
                              <th align=center style='font-size: ".$this->header_font_size."'>Include In<br>Global Search</th>
						<th align=center style='font-size: ".$this->header_font_size."'>Link<br>Descriptor</th>
						<th align=center style='font-size: ".$this->header_font_size."'>Node<br>Type</th>
					</tr>
					<tr>$checkboxes</tr>
				</table>\n";
				return $return_data;
		
		}

          // obtains the header/footer field so the user can add text to the generated nodes
          function get_note_field($field_name, $label, $width="400px", $height="100px"){
               $field_data = "<b>".$label."</b><br>
                         <input type=\"hidden\" id=\"".$field_name."\" name=\"".$field_name."\" value=\"\" style=\"display:none\" />
                         <input type=\"hidden\" id=\"".$field_name."___Config\" value=\"AutoDetectLanguage=false&amp;DefaultLanguage=en\" style=\"display:none\" />
                         <iframe id=\"".$field_name."___Frame\" src=\"./atk/attributes/fck/editor/fckeditor.html?InstanceName=".$field_name."&amp;Toolbar=Basic\" width=\"".$width."\" height=\"".$height."\" frameborder=\"no\" scrolling=\"no\"></iframe>
                    ";

               $return_data= $field_data;//$this->get_content_div($field_data, $label, "style='width:100px; font-size: 0.9em'", "style='width:".$width.";'");

               return $return_data;
               
          } // end function
          
          
		// obtains all the attributes for the given node
		function get_attribute_format($node_name){
			$attribute_position = 0;
			$attributes_count = count($this->attribute_flags_array);
               $m2o_count = 0; // keep track of m2o to signal possible intermediary node if > 1
               
               // get the header/footer notes field
               if($this->enable_node_notes){
                    $tip_note = "<br><b>Note:</b> If you want to show the font color and font size on the toolbar, replace the basic toolset on the file ./atk/attributes/fck/fckconfig.js with:<br>
                                   FCKConfig.ToolbarSets[\"Basic\"] = [	['Bold','Italic','-','OrderedList','UnorderedList','-','Link','Unlink','-','About','TextColor','FontFormat']     ] ;
                    
                    ";


                    $header_note = $this->get_note_field($node_name."_header_note","Add Header Note For <font color='#CE0000'><b>".$node_name."</font>","640px","80px");
                    $footer_note = $this->get_note_field($node_name."_footer_note","Add Footer Note For <font color='#CE0000'><b>".$node_name."</font>","640px","150px").$tip_note;
               }
               
			$return_data = $header_note."		
				<table border='1' cellspacing='0' cellpadding='0' class='recordlist'>
				  <tr>
				    <td colspan='".($attributes_count+6)."' ><h3>Attributes for <font color='#CE0000'><b>$node_name</b></font></h3></td>
				  </tr>
		            <!-- start header -->
		            <tr>					      
			      <th valign='middle' style='font-size: ".$this->header_font_size."'>Attribute</th>      
				 <th valign='middle' style='font-size: ".$this->header_font_size."'>Position</th>
				 <th valign='middle' style='font-size: ".$this->header_font_size."'>Tab<br>Name</th>      
					";
				// print attribute header
				for($i=0; $i < $attributes_count; $i++){
					$label = $this->attribute_flags_array[$i]->getLabel();
					$return_data .= "
					  <th valign='middle' style='font-size: ".$this->header_font_size."' > ".str_replace(" ","<br>",$label)." </th>
				  ";
				}// end for

     	          $return_data .= "
     			  	<th valign='middle' style='font-size: ".$this->header_font_size."' >Relationships</th>      
                         <th valign='middle' style='font-size: ".$this->header_font_size."'>DB<br>Type</th>      
     			  	<th valign='middle' style='font-size: ".$this->header_font_size."'>ATK<br>Type</th>      
			  </tr>
				<!-- end header -->
				";

	               $db = atkGetDb();               
	               $query = "SHOW FIELDS FROM $node_name";
	
	               //$db->query($query);
	               $rows = $db->getrows($query);
	               $total_rows = count($rows);
				$counter = 0;
	               $data = array();

				$found_first_searchable  = false;
				$found_first_order 	 	= false;
				$found_first_index 	 	= false;
				$found_first_descriptor 	= false;				
				$relationships_array 	= array();
				
				
                    // get all relationships for this table
				$relationships = $this->get_relations($node_name, $total_rows, $relationships_array);
                    
	               //while ($db->next_record())
	               // traverse through all table fields
				for($r=0; $r < $total_rows; $r++)
	               {
	     			// get field properties from database
					$field_name = $rows[$r]["Field"];
	     			$field_type = $rows[$r]["Type"];
	     			$field_null = $rows[$r]["Null"]; // YES
	     			$field_key = $rows[$r]["Key"]; // PRI
	     			$field_default = $rows[$r]["Default"];
	     			$field_extra = $rows[$r]["Extra"]; // auto_increment

					$field_options_array = array();
					$atk_field_type = $this->get_atk_field_mapper($field_name,$field_type, $field_options_array);
					$db_field_type = $field_type;
					
					if((($atk_field_type == $this->attributes["radio"]) && ($field_options_array["type"]=="enum")) || (($atk_field_type == $this->attributes["multilist"]) && ($field_options_array["type"]=="set"))){
						$db_field_type = $field_options_array["value"];
					}// end if
					 
					$hidden_field_name = "";
					$control_field_name = $node_name."*".$field_name."_atk_attribute_type"; 

					if(($field_options_array["type"]!= "text") && ($field_options_array["type"]!= "varchar")&& ($field_options_array["type"]!= "enum") && ($field_options_array["type"]!= "set")){
						$hidden_field_name = "<input type='hidden' name='".$control_field_name."' value='$atk_field_type'>";
					}
					
					$disabled = "";

					if((strtolower($field_key) == "pri") || (strtolower($field_key) == "mul")){ 
						$disabled = "disabled";
					}

                         $tr_row = (($r%2)==0)?2:1;						
					$return_data .= "
					  		<input type='hidden' name='".$node_name."_attribute[]' value='$field_name'>
					  		<!-- input type='hidden' name='".$node_name."*".$field_name."_atk_attribute_type' value='$atk_field_type' -->
					  		".$hidden_field_name."
							<input type='hidden' name='".$node_name."*".$field_name."_db_attribute_type' value=\"$db_field_type\">
						     <input type='hidden' name='".$node_name."*".$field_name."_position_orig' size='5' value='".$attribute_position."'>
							<tr  valign='top'
                                             id=\"".$this->form_name."_".$i."\" 
                                              class=\"row".$tr_row."\"                    
                                              onmouseover=\"highlightrow(this, '#F6A03D')\"
                                             onmouseout=\"resetrow(this)\"
                                             onclick=\"selectrow(this, '".$this->form_name."', ".$i.")\"
                                   
                                    >
							     <td align=left>$field_name</td>
							     <td align=left><input type='text' name='".$node_name."*".$field_name."_position' size='5' $disabled value='".$attribute_position."'></td>
							     <td align=left><input type='text' name='".$node_name."*".$field_name."_tab_name' size=12 $disabled></td>
						    ";

					$attribute_position += 100;

                         $searchable_field_code=""; // used for field color display if toggle field available

                         // traverse through all defined attribute flags and options
					for($i=0; $i < $attributes_count; $i++){
						$name 		= $this->attribute_flags_array[$i]->getFlagName();
						$label 		= $this->attribute_flags_array[$i]->getLabel();
						$value	 	= $this->attribute_flags_array[$i]->getValue();
						$type 		= $this->attribute_flags_array[$i]->getType();
						$disabled 	= "";
						$primary_field = "";

						// primary key
						if((strtolower($field_key) == "pri") && ($name=="AF_PRIMARY|AF_HIDE")){
							$disabled = "disabled";
							$value = 1;
							$primary_field = "<input type='hidden' name='".$node_name."_primary_field_attribute' value='$field_name'>\n";
						}else if((strtolower($field_key) == "pri") || ($name=="AF_PRIMARY|AF_HIDE")){
							$disabled = "disabled";
							$value = 0;						
						}
						
						// m2o/AF_RELATION_AUTOLINK relation
						if(($name=="AF_RELATION_AUTOLINK") && ($relationships_array[$field_name]=="m2o")){
							$value = 1;
							$m2o_count++;
						}else if($name=="AF_RELATION_AUTOLINK"){
							$disabled = "disabled";
						}

						// m2o/AF_LARGE relation
						if(($name=="AF_LARGE") && ($relationships_array[$field_name]=="m2o")){
							$value = 0;
							//$m2o_count++;
						}else if($name=="AF_LARGE"){
							$disabled = "disabled";
						}

						// o2m relation
						if(($name=="AF_ONETOMANY_EMBED") && ($relationships_array[$field_name]=="o2m")){
							$value = 1;
							$disabled = "enabled";
						}else if($name=="AF_ONETOMANY_EMBED"){
							$disabled = "disabled";
						}

						// null
						if((empty($field_null)) && ($name=="AF_OBLIGATORY")){
							$disabled = "disabled";
							$value = 1; // checked
						}
						
						// - make created_date/created_by hide add, hide list, read only
						if((strtolower($field_name) == "created_date") || (strtolower($field_name) == "created_by") || (strtolower($field_name) == "created_by_id")){
							if(($name == "AF_HIDE_ADD") || ($name == "AF_HIDE_LIST") || ($name == "AF_READONLY")){
								$value = 1; // checked							
							}

                                   // hide field completely. added 11/16/06
                                   if((strtolower($field_name) == "created_by_id") && ($name == "AF_HIDE")){
                                   	$value = 1; // checked
                                   }

						}

						// - make modified_date hide add, hide list, read only
						if((strtolower($field_name) == "modified_date") || (strtolower($field_name) == "modified_by") || (strtolower($field_name) == "modified_by_id")){
							if(($name == "AF_HIDE_ADD") || ($name == "AF_HIDE_LIST") || ($name == "AF_READONLY") || ($name == "__forceupdate__")){
								$value = 1; // checked							
							}
                                   
                                   // hide field completely. added 11/16/06
                                   if((strtolower($field_name) == "modified_by_id") && ($name == "AF_HIDE")){
                                   	$value = 1; // checked
                                   }
						}


						// make url fields selected popup 
						if((strstr($field_type, "varchar")) && ($name == "AF_POPUP") &&  (strstr($field_name, "url"))){
							$value = 1; // checked							
						}						


						// make first varchar field searchable
						if((!$found_first_searchable) && (strstr($field_type, "varchar")) && ($name == "AF_SEARCHABLE") ){
							$value = 1; // checked							
							$found_first_searchable = true;
							$searchable_field_code = "\n<input type='hidden' name='".$node_name."_color_attribute' value='$field_name'>\n";
						}						


						// make first varchar field order
						if((!$found_first_order) && (strstr($field_type, "varchar")) && ($name == "__order__") ){
							$value = 1; // checked							
							$found_first_order = true;
						}						

						// make first varchar field index
						if((!$found_first_index) && (strstr($field_type, "varchar")) && ($name == "__index__") ){
							$value = 1; // checked							
							$found_first_index = true;
						}						

                              // detect if we have parent fields
                              if(array_key_exists($field_name,$this->parent_fields_array["field"])){
                                   $this->parent_fields_array["state"] = true;
                              }
                              
						// make first varchar field descriptor
						if((!$found_first_descriptor) && (strstr($field_type, "varchar")) && ($name == "__descriptor__")  && ((array_search("o2m",$relationships_array)) || ($this->parent_fields_array["state"]) || ($this->function_includes_array["descriptor"])) ){//
							$value = 1; // checked							
							$found_first_descriptor = true;
						}						


						// hide text fields from list view
						if( (strstr($field_type, "text")) && ($name == "AF_HIDE_LIST") ){
							$value = 1; // checked							
							$found_first_searchable = true;
						}						

						// hide user_id field
						if(($field_name == $this->user_field) &&  ($name == "AF_HIDE") && (strstr($field_type, "int"))){
							$value = 1; // checked
						}
						
						if(($type == "radio") && ($name == "__index__")){
							$name  = $field_name;
							$field_option_name = $node_name."_index_attribute";
						}else if(($type == "radio") && ($name == "__descriptor__")){
							$name  = $field_name;
							$field_option_name = $node_name."_descriptor_attribute";
						}else if(($name == "__order__")){
							$name  = $field_name;
							$field_option_name = $node_name."_order_attribute[]";						
						}else if(($name == "__forceupdate__")){
							$name  = $field_name;
							$field_option_name = $node_name."_forceupdate_attribute[]";						
						}else{
							$field_option_name = $node_name."*".$field_name."_attribute_options[]";
						}
						
						
						$checked = ($value==1)?"checked":"";
	
						$return_data .= $primary_field."
						    <td align=left>&nbsp;<input type='$type' value='$name' name='".$field_option_name."' $checked ></td>
					  ";
	
					  }// end for

                         // add color field 
                         $return_data .= $searchable_field_code;

                         // add relationship column
                         if($r==0){                         
     					$return_data .= $relationships;
                         }

					if(empty($hidden_field_name)){ // either type text or varchar
						$atk_field_type_list = $this->get_related_attribute_types($atk_field_type, $field_options_array["type"],$control_field_name);

						// get optional folder name for file types
                              if($atk_field_type == $this->attributes["file"]){
                                   $file_folder = "<b>Folder Name:</b> <input type=text name='".$node_name."*".$field_name."_attribute_file_folder' value='".$this->attachment_files_folder."'>\n";
                                   $file_flags = $this->get_file_attribute_flags($field_name, $node_name);
                                   if(!empty($file_flags)){
                                       $file_flags=  "<b>File Flags</b><br>".$file_flags;
                                   }
                                        //print "file_flags=$file_flags<br>";
                                   $atk_field_type_list .= $this->get_content_div($file_folder."<br>".$file_flags, "Other Options", "style='font-size: 0.9em; width: 100px'");
                              }

						// get optional folder name for file types
                              if($atk_field_type == $this->attributes["fck"]){
                                   $fck_options = "<b>FCK Toolbar Type:</b><br>\n";
                                   $fck_options .= $this->get_fck_attribute_options($field_name, $node_name);
                                   //if(!empty($file_flags)){
                                     //  $file_flags=  "<b>File Flags</b><br>".$file_flags;
                                   //}
                                        //print "file_flags=$file_flags<br>";
                                   $atk_field_type_list .= $this->get_content_div($fck_options, "FCK Options", "style='font-size: 0.9em; width: 80px'");
                              }
                              
                              $atk_field_type = $atk_field_type_list;
                              
					}else if($atk_field_type == $this->attributes["currency"]){ // added 12/22/06
                              $atk_field_type_list = $this->get_related_attribute_types($atk_field_type, $field_options_array["type"],$control_field_name);                         
                              $atk_field_type = $atk_field_type_list;
                         }
					
					$return_data .= "
						    <td align=left>&nbsp;".$field_type."</td>
						    <td align=left>&nbsp;".$atk_field_type."</td>
						  </tr>
					  ";

	                    $counter++;
	               }// end for 

				$return_data .= "
						<input type='hidden' name='".$node_name."_last_position' value='".$attribute_position."'>
						";
                    // added 10/21/06 to add display checkbox on interface, if node has more than 1 m2o relation
                    // when the checkbox is selected, code is added to automatically pull the latest primary values of the side nodes 
                    if($m2o_count>1){
                         // adds a checkbox and if selected, we add preAdd() and getSideNodesId()
                         $return_data .= "<tr><td colspan=".($attributes_count + 6)." bgcolor='#FFDE00'><input type='checkbox' name='".$node_name."_node_intermediary' value='1'>Intermediary Node. <i>Select check box on left to add code that will automatically retrieve the latest primary field values of the side nodes</i></td></td>\n";
                    }// end if
		
				$return_data .= "</table>\n";
				$return_data .= $footer_note."\n";
                    
                    return $return_data;
		
		}// end function
          
          // gets all the flags specific to file attribute
          function get_file_attribute_flags($field_name, $node_name){
               $return_value = "";
               $attributes_count = count($this->file_attribute_flags_array);               
			$field_option_name = $node_name."*".$field_name."_attribute_options[]";
		        //print "field_option_name= $field_option_name<br>$attributes_count<br>";
               // traverse through all defined attribute flags and options
			for($i=0; $i < $attributes_count; $i++){
				$name 		= $this->file_attribute_flags_array[$i]->getFlagName();
				$label 		= $this->file_attribute_flags_array[$i]->getLabel();
				$value	 	= $this->file_attribute_flags_array[$i]->getValue();
				$type 		= $this->file_attribute_flags_array[$i]->getType();
				$disabled 	= "";

				$checked = ($value==1)?"checked":"";

				$return_value .= "
				    &nbsp;<input type='$type' value='$name' name='".$field_option_name."' $checked $disabled>&nbsp;&nbsp;".$label."<br>\n
				    ";
               }// end for
               return $return_value;
          }// end function


          
          // gets all the flags specific to file attribute
          function get_fck_attribute_options($field_name, $node_name){
               $return_value = "";
               $attributes_count = count($this->fck_attribute_options_array["toolbar"]);               
			$field_option_name = $node_name."*".$field_name."_attribute_toolbar";
		        //print "field_option_name= $field_option_name<br>$attributes_count<br>";
               // traverse through all defined attribute flags and options
			for($i=0; $i < $attributes_count; $i++){
				$name 		= $this->fck_attribute_options_array["toolbar"][$i];
				$label 		= $name; //$this->file_attribute_flags_array[$i]->getLabel();
				$value	 	= $name;//$this->file_attribute_flags_array[$i]->getValue();
				$type 		= "radio";//$this->file_attribute_flags_array[$i]->getType();
				$disabled 	= "";

				$checked = (strtolower($value)=="basic")?"checked":"";

				$return_value .= "
				    &nbsp;<input type='$type' value='$name' name='".$field_option_name."' $checked $disabled>&nbsp;&nbsp;".$label."<br>\n
				    ";
               }// end for
               return $return_value;
          }// end function
          
          function get_related_attribute_types($atk_type, $db_type, $control_field_name){
			$return_data = "<select name='$control_field_name'>\n";
			$return_data .= "<option value='$atk_type'>$atk_type</option>\n"; // set provide type as default

			if(array_key_exists($db_type,$this->related_attributes_array)){
				for($i=0; $i< count($this->related_attributes_array[$db_type]); $i++){
					$current_type = $this->related_attributes_array[$db_type][$i];
					if($current_type != $atk_type){ // exclude provide type
						$return_data .= "<option value='".$current_type."'>".$current_type."</option>\n";
					}// end if
				}// end for
			
			}
			$return_data .= "</select>\n";
			return $return_data;
		}// end function
		
          // shows initial database tables on system
          function print_tables_form($data){
               if(!is_array($data)){return;}

                $ui = &$this->getUi();
                $this->addStyle("style.css");
                $this->addStyle("recordlist.css");
                
                
                if (is_object($ui))
                {
                    $page = &$this->getPage();
                    $page->head(atktext("dashboard"));
                    $page->body();
                    //$page->register_script(moduleDir("webstoredl")."js/test.js");
                    $page->register_script("./atk/javascript/tools.js");
                    $page->register_script("./atk/javascript/recordlist.js");
                    $page->register_script("./atk/javascript/formselect.js");


                    $content = 
                    					$this->getJSCustom().
                         "

                         <table width=100% class='recordlist' cellspacing=0> 
                              <form  name='".$this->form_name."' method=post  onKeyUp='highlight(event)' onClick='highlight(event)'>
                                <input type='hidden' name='atkprevlevel' value='0'>
                                <input type='hidden' name='atkstackid' value='445670af19b1b'>
                                <input type='hidden' name='atkapp' value='99b165f709d98684780c9bc0596c237d'>
                                <input type='hidden' name='atkescape' value=''>
                                <input type='hidden' name='atknodetype' value='".$this->module.".".$this->node."'>
                                <input type='hidden' name='atkaction' value='".$this->form_action["next"]."'>
                                <script language='javascript' type='text/javascript'>var ".$this->form_name." = new Object();</script>

				            <!-- start header -->
				            <tr>					      
					      <th align='left' valign='middle' >                  #                </th>
					      <th align='left' valign='middle' >                  Table Name                </th>
				            </tr>
							<!-- end header -->
							<!-- start table body added 1/19/07 -->
                                   <tbody style='height:390px; overflow: auto;'>
                         \n";
                         
                    for($i=0;$i<count($data); $i++){
                         $table_name = $data[$i];
                         $tr_row = (($i%2)==0)?2:1;
                         $content .= "<tr
                                             id=\"".$this->form_name."_".$i."\" 
                                              class=\"row".$tr_row."\"                    
                                              onmouseover=\"highlightrow(this, '#999999')\"
                                             onmouseout=\"resetrow(this)\"
                                             onclick=\"selectrow(this, '".$this->form_name."', ".$i.")\"
                         
                                        ><td align=left>".($i + 1)."
								<td align=left>
                                              <input type='checkbox' name='".$this->form_name."_atkselector[]' value='$table_name' onclick=\"if (this.disabled) this.checked = false\">$table_name
                                             </td></tr>\n";
                    }// end for

                    $content .= "

                              </tbody>
                              <!-- end table body -->
				            <!-- start footer -->
				            <tr>					      
					      <th valign='middle' > &nbsp;</th>
					      <th valign='middle' > &nbsp;</th>
				            </tr>
							<!-- end footer -->

                        <!-- multirecord actions -->
                       <tr>
                         <td align='left'><td align='left'>

                           <table border='0' cellspacing='0' cellpadding='2'>
                             <tr>
                               <td valign='top'><img src='./atk/images/arrow.gif' border='0'></td>
                               <td>
                                 <a href=\"javascript:updateSelection('".$this->form_name."', document.forms['".$this->form_name."'], 'all')\">Select all</a> / <a href=\"javascript:updateSelection('".$this->form_name."', document.forms['".$this->form_name."'], 'none')\">Deselect all</a> / <a href=\"javascript:updateSelection('".$this->form_name."', document.forms['".$this->form_name."'], 'invert')\">Invert selection</a>&nbsp; <input type='hidden' name='".$this->form_name."_atkaction' value='".$this->form_action["next"]."'><input type='button' class='btn btn-primary' value='".$this->form_action["next"]."' onclick=\"atkSubmitMRA('".$this->form_name."', this.form, '".$this->dispatch_url."')\"> 
                               </td>
                             </tr>

                           </table>
                         </td>
                       </tr>
                      </table><br></td>
                      </tr>
                    \n";
   
                    $content .= "</form>\n</table>\n";

                    $version_info = $this->get_version();
                    $content .= $version_info;
                    $content_box = $ui->renderBox(array("title"=>atktext("Existing tables for database: ".$this->db_name),
                                                         "content"=>$content)
                                                         
                                                         );

                  $page->addContent($content_box);

               }// end if (is_object($ui))
          }// end function
          
          function get_version(){
               //$version_info = "<br><b>ATK Code Generator Version:</b> ".ACG_VERSION."<br><b>Current ATK version:</b> ".$this->atk_version;
               $version_info = "<table width='100%'cellspacing=5><td><b>ATK Code Generator Version:</b> ".ACG_VERSION."<td><b>Current ATK version:</b> ".$this->atk_version."<td><b>Developed by:</b> Jorge Garifuna (<a href='mailto:info@GariDigital.com'>info@GariDigital.com</a> |  <a target='_blank' href='http://www.GariDigital.com'>GariDigital.com</a> | <a target='_blank' href='http://www.GariSoft.com'>GariSoft.com</a>)</tr></table>";
               return $version_info; 
          }
          function adminHeader(){
               $help_text = "
                    <h1><font color=#FF6347>Budget</font></h1>
               ";
               return $help_text;
          }// end function

          function adminFooter(){
               $help_text = "
                    <h2>Legend</h2>
                    <b>Years:</b> - The number of years of the term of the loan<br>
                    <b>Interest:</b> - The interest rate to be paid on the loan<br>
                    <b>Loan Amount:</b> - The amount of the loan <br>
                    <b>Annual Tax:</b> - The annual amount of taxes to pay on the loan in currency (not percentages)<br>
                    <b>Annual Insurance:</b> - The annual amount of insurance to pay on the loan in currency (not percentages)<br>
                    <b>Payment Amount</b> - The calculated monthly to pay <br>
                    <b>Payment Tax Amount:</b> - The calculated monthly tax amount to pay on the loan in currency (not percentages)<br>
                    <b>Payment insurance amount: </b> - The calculated monthly amount to pay for insurance on the loan in currency (not percentages)<br><br>
                    <b>Total Payment:</b> - The calculated total monthly amount to pay on the loan including taxes and insurance<br>                    <br>                    
                    
                    <b>Author:</b> - Jorge Garifuna (<a href='http://www.GariDigital.com' target=_blank>GariDigital.com</a>) <<a href='mailto:info@GariDigital.com'>info@GariDigital.com</a>> <br>
                    <b>Version:</b> - 1.0 <br>
                    <b>Originally Created:</b> - March 24, 2006 <br>
                    <b>Updated:</b>  <br>
               ";
               return $help_text;
          }// end function
          
          // maps db field types to ATK attributes
          function get_atk_field_mapper($field_name, $field_type, &$field_options_array){
			// check field lenth
			if(strpos($field_type,"(")){
				$field_type = str_replace("(","�",$field_type); //�
				$field_type = str_replace(")","",$field_type);
				list($field_type, $field_type_len) = explode("�",$field_type);
				$field_options_array["value"]	= $field_type_len;

				// check if we have a comma on field type len
				if(strpos($field_type_len,",")){
					list($field_type_len, $field_type_len2) = explode(",",$field_type_len);
				}
				$field_options_array["type"]	= $field_type;
			}else{
				$field_options_array["type"]	= $field_type;
			}
			
			if($field_type == "int"){
                    if($field_type_len == 1){
					$return_value = $this->attributes["bool"];
				}else{
				      $return_value = $this->attributes["number"];
				}
			}else if($field_type == "decimal"){
				$return_value = $this->attributes["currency"];
			}else if($field_type == "tinyint"){
				if($field_type_len == 1){
					$return_value = $this->attributes["bool"];
				}else{
					$return_value = $this->attributes["number"];
				}
				
			}else if($field_type == "char"){
				//if($field_type_len == 1){
				//	$return_value = $this->attributes["radio"];
				//}else{
					$return_value = $this->attributes["attribute"];
				//}
			}else if(($field_type == "varchar") && ( (strstr($field_name,"image")) || (strstr($field_name,"attachment")) || (strstr($field_name,"photo")) || (strstr($field_name,"file")) )){
				$return_value = $this->attributes["file"];
			}else if(($field_type == "varchar") && ( (strstr($field_name,"color")) )){
				$return_value = $this->attributes["color"];
			}else if(($field_type == "varchar") && ( (strstr($field_name,"email")) )){
				$return_value = $this->attributes["email"];
			}else if(($field_type == "varchar") && ( (strstr($field_name,"url")) )){
				$return_value = $this->attributes["url"];
			}else if(($field_type == "varchar") && ( (strstr($field_name,"password")) )){
				$return_value = $this->attributes["password"];
			}else if(($field_type == "varchar") && ( (strstr($field_name,"country")) )){ // added 12/30/06
				$return_value = $this->attributes["country"];
			}else if(($field_type == "varchar") && ( (strstr($field_name,"state")) )){ // added 12/30/06
				$return_value = $this->attributes["state"];
			}else if(($field_type == "varchar") && ( (strstr($field_name,"duration")) )){ // added 12/30/06
				$return_value = $this->attributes["duration"];
			}else if($field_type == "time"){ // added 12/30/06
                         $return_value = $this->attributes["time"];                    
			}else if($field_type == "date"){
				$return_value = $this->attributes["date"];
			}else if($field_type == "datetime"){
				$return_value = $this->attributes["datetime"];
			}else if($field_type == "text"){
				//$return_value = $this->attributes["text"];
                    $return_value = $this->attributes["fck"];
			}else if($field_type == "float"){
				$return_value = $this->attributes["number"];
			}else if($field_type == "smallint"){
				$return_value = $this->attributes["number"];
			}else if($field_type == "enum"){
				$return_value = $this->attributes["list"]; // changed from radio to list on 12/30/06
			}else if($field_type == "set"){
				$return_value = $this->attributes["multilist"];
			}else{
				$return_value = $this->attributes["attribute"];
			}
			return $return_value;
		}// end function

		// obtains javascript code to enable all checkboxes prior to submission and check for module name (5/26/06)
		function getJSCustom(){
		
			$return_value = $this->getCSSCustom(). 
               
                    "
				<script language=\"javascript\">


                         // manages relationshit checkboxes
                         // for example when o2m is selected, shuffle and many bool relation are disabled and visaversa
               		function manageRelationCheckboxes(object){
                    		  var my_name = object.name;
                    		  var my_value = object.value;
                    		  var my_checked = object.checked;
                    		  var total_fields = document.forms[0].elements[my_name].length;
                                var my_values_array = my_value.split('*');
                    
                                if (total_fields > 0) 
                    		  {				
                    		    var field = document.forms[0];
                    		    for (i = 0; i < total_fields; i++) 
                    		    {      
                                        var field = document.forms[0].elements[my_name][i];
                                        var value = document.forms[0].elements[my_name][i].value;
                                        var checked = document.forms[0].elements[my_name][i].checked;
                                        var disabled = document.forms[0].elements[my_name][i].disabled;
                                        var values_array = value.split('*');
                                        if((values_array[4] != '".$this->relation_type_array["m2o"]."') && (my_values_array[2] == values_array[2])){ // exclude m2o relations
                                             if(my_checked && (my_value != value)){
                                                  field.disabled = true;                                   
                                             }else{
                                                  field.disabled = false;
                                             }                                   
                                        }
                    		    }// end for
                    		    //alert(rv);
                    			return true; //checkModuleName();
                    		  }// end if
                    		  return false;
                         }// end function
                         


					// enables all checkboxes prior to submission so that the backend script can read them
                         function enableCheckboxes()
					{
					  if (document.forms.length > 0) 
					  {
						if(!checkModuleName()){ // check module name
							return false;
						}
						
					    var field = document.forms[0];
					    var rv = \"\";
					    for (i = 0; i < field.length; i++) 
					    {      
							if(field.elements[i].type==\"checkbox\"){
								//rv += i+\") (checked: \"+field.elements[i].checked+\") [disabled: \"+field.elements[i].disabled+\"]\\n\";
								if(field.elements[i].checked && field.elements[i].disabled){
									field.elements[i].disabled = false;
								}				
							}
					    }// end for
					    //alert(rv);
						return true; //checkModuleName();
					  }// end if
					  return false;
					}// end function

					function checkModuleName(){
						var mvalue = document.forms[0].elements[\"module_name\"].value;
						if(mvalue == \"\"){
							alert(\"Please enter a module name\");
							document.forms[0].elements[\"module_name\"].focus();
							return false;
						}else{return true;}	
					}

                         // start form hightlight script
                         var highlightcolor=\"lightgreen\"
                         
                         var ns6=document.getElementById&&!document.all
                         var previous=''
                         var eventobj
                         
                         //Regular expression to highlight only form elements
                         var intended=/INPUT|TEXTAREA|SELECT|OPTION/
                         
                         //Function to check whether element clicked is form element
                         function checkel(which){
                         if (which.style&&intended.test(which.tagName)){
                         if (ns6&&eventobj.nodeType==3)
                         eventobj=eventobj.parentNode.parentNode
                         return true
                         }
                         else
                         return false
                         }
                         
                         //Function to highlight form element
                         function highlight(e){
                         eventobj=ns6? e.target : event.srcElement
                         if (previous!=''){
                         if (checkel(previous))
                         previous.style.backgroundColor=''
                         previous=eventobj
                         if (checkel(eventobj))
                         eventobj.style.backgroundColor=highlightcolor
                         }
                         else{
                         if (checkel(eventobj))
                         eventobj.style.backgroundColor=highlightcolor
                         previous=eventobj
                         }
                         }
                         // end form hightlight script


                         
                         // START JS CODE FOR HIDE STYLE of: http://www.dhtmlgoodies.com/index.html?whichScript=show_hide_content_slide
                         
                         
                         /************************************************************************************************************
                         (C) www.dhtmlgoodies.com, November 2005
                         
                         This is a script from www.dhtmlgoodies.com. You will find this and a lot of other scripts at our website.	
                         
                         Terms of use:
                         You are free to use this script as long as the copyright message is kept intact. However, you may not
                         redistribute, sell or repost it without our permission.
                         
                         
                         Thank you!
                         
                         www.dhtmlgoodies.com
                         Alf Magne Kalleland
                         
                         Updated:
                         	April, 3rd, 2006: Fixed problem with initial sliding in IE.
                         
                         ************************************************************************************************************/
                         
                         var dhtmlgoodies_slideSpeed = 10;	// Higher value = faster
                         var dhtmlgoodies_timer = 10;	// Lower value = faster
                         
                         var objectIdToSlideDown = false;
                         var dhtmlgoodies_activeId = false;
                         function showHideContent(e,inputId)
                         {
                         	if(!inputId)inputId = this.id;
                         	inputId = inputId + '';
                         	var numericId = inputId.replace(/[^0-9]/g,'');
                         	var answerDiv = document.getElementById('dhtmlgoodies_a' + numericId);
                         	
                         	objectIdToSlideDown = false;
                         	
                         	if(!answerDiv.style.display || answerDiv.style.display=='none'){
                         
                         		if(dhtmlgoodies_activeId &&  dhtmlgoodies_activeId!=numericId){			
                         			objectIdToSlideDown = numericId;
                         			slideContent(dhtmlgoodies_activeId,(dhtmlgoodies_slideSpeed*-1));
                         		}else{
                         			
                         			answerDiv.style.display='block';
                         			answerDiv.style.visibility = 'visible';
                         			
                         			slideContent(numericId,dhtmlgoodies_slideSpeed);
                         		}
                         	}else{
                         		slideContent(numericId,(dhtmlgoodies_slideSpeed*-1));
                         		dhtmlgoodies_activeId = false;
                         	}	
                         }
                         
                         function slideContent(inputId,direction)
                         {
                         	var obj =document.getElementById('dhtmlgoodies_a' + inputId);
                         	var contentObj = document.getElementById('dhtmlgoodies_ac' + inputId);
                         	height = obj.clientHeight;
                         	height = height + direction;
                         	rerunFunction = true;
                         	if(height>contentObj.offsetHeight){
                         		height = contentObj.offsetHeight;
                         		rerunFunction = false;
                         	}
                         	if(height<=1){
                         		height = 1;
                         		rerunFunction = false;
                         	}
                         
                         	obj.style.height = height + 'px';
                         	var topPos = height - contentObj.offsetHeight;
                         	if(topPos>0)topPos=0;
                         	contentObj.style.top = topPos + 'px';
                         	if(rerunFunction){
                         		setTimeout('slideContent(' + inputId + ',' + direction + ')',dhtmlgoodies_timer);
                         	}else{
                         		if(height<=1){
                         			obj.style.display='none'; 
                         			if(objectIdToSlideDown && objectIdToSlideDown!=inputId){
                         				document.getElementById('dhtmlgoodies_a' + objectIdToSlideDown).style.display='block';
                         				document.getElementById('dhtmlgoodies_a' + objectIdToSlideDown).style.visibility='visible';
                         				slideContent(objectIdToSlideDown,dhtmlgoodies_slideSpeed);				
                         			}
                         		}else{
                         			dhtmlgoodies_activeId = inputId;
                         		}
                         	}
                         }
                         
                         
                         
                         function initShowHideDivs()
                         {
                         	var divs = document.getElementsByTagName('DIV');
                         	var divCounter = 1;
                         	for(var no=0;no<divs.length;no++){
                         		if(divs[no].className=='dhtmlgoodies_question'){
                         			divs[no].onclick = showHideContent;
                         			divs[no].id = 'dhtmlgoodies_q'+divCounter;
                         			
                         			var answer = divs[no].nextSibling;
                         			while(answer && answer.tagName!='DIV'){
                         				answer = answer.nextSibling;
                         			}
                         			
                         			answer.id = 'dhtmlgoodies_a'+divCounter;	
                         			
                         			contentDiv = answer.getElementsByTagName('DIV')[0];
                         			contentDiv.style.top = 0 - contentDiv.offsetHeight + 'px'; 	
                         			contentDiv.className='dhtmlgoodies_answer_content';
                         			contentDiv.id = 'dhtmlgoodies_ac' + divCounter;
                         			answer.style.display='none';
                         			answer.style.height='1px';
                         			divCounter++;
                         		}		
                         	}	
                         }
                         window.onload = initShowHideDivs;
                         
                         // END JS CODE FOR HIDE STYLE of: http://www.dhtmlgoodies.com/index.html?whichScript=show_hide_content_slide

					</script>			
			";
			
			return $return_value;
		}// end function getJSCustom()

		// obtains custom CSS for js sections above
		function getCSSCustom(){
		
			$return_value = "
                              <style type=\"text/css\">
                              /*
                              
                              START sliding style for hide css from:
                              http://www.dhtmlgoodies.com/index.html?whichScript=show_hide_content_slide
                              body{
                              	font-family: Trebuchet MS, Lucida Sans Unicode, Arial, sans-serif;*/	/* Font to use */
                              /*	margin:0px;
                              	
                              }*/
                              
                              .dhtmlgoodies_question{	/* Styling question */
                              	/* Start layout CSS */
                              	color:#FFF;
                              	font-size:0.6em;
                              	background-color:#317082;
                              	width:30px;//430
                              	margin-bottom:2px;
                              	margin-top:2px;
                              	padding-left:2px;
                              	background-image:url('images/bg_answer.gif');
                              	background-repeat:no-repeat;
                              	background-position:top right;	
                              	height:15px;
                              	
                              	/* End layout CSS */
                              	
                              	overflow:hidden;
                              	cursor:pointer;
                              }
                              .dhtmlgoodies_answer{	/* Parent box of slide down content */
                              	/* Start layout CSS */
                              	border:1px solid #317082;
                              	background-color:#E2EBED;
                              	width:400px;
                              	
                              	/* End layout CSS */
                              	
                              	visibility:hidden;
                              	height:0px;
                              	overflow:hidden;
                              	position:relative;
                              
                              }
                              .dhtmlgoodies_answer_content{	/* Content that is slided down */
                              	padding:1px;
                              	font-size:0.9em;	
                              	position:relative;
                              }
                              
                              /* END sliding style for hide css from: */
                              </style>
                              
			
			";
			
			return $return_value;
		}// END FUNCTION getCSSCustom()

		
  }// end main class

     /*
     
          SUPPORT CLASSES
     */
     // container class for node flags
	class node_flags{
		var $flag_name;
		var $label;
		var $type; // radio, checkbox
		var $default_value; // 1 or 0

		function __construct($flag_name, $label, $type="checkbox", $default_value=1){
			$this->flag_name = $flag_name;
			$this->label = $label;
			$this->type = $type;
			$this->default_value = $default_value;
		}// end function

		function node_flags($flag_name, $label, $type="checkbox", $default_value=1){
			$this->flag_name = $flag_name;
			$this->label = $label;
			$this->type = $type;
			$this->default_value = $default_value;
		}// end function
		
		function getFlagName(){			return $this->flag_name;		}		
		function getLabel(){			return $this->label;		}
		function getType(){			return $this->type;		}
		function getValue(){			return $this->default_value;		}
	}// end class node_flags
	
     // container class for attribute flags
	class attribute_flags{
		var $flag_name;
		var $label;
		var $type; // radio, checkbox
		var $default_value; // 1 or 0

		function __construct($flag_name, $label, $type="checkbox", $default_value=1){
			$this->flag_name = $flag_name;
			$this->label = $label;
			$this->type = $type;
			$this->default_value = $default_value;
		}// end function

		function attribute_flags($flag_name, $label, $type="checkbox", $default_value=1){
			$this->flag_name = $flag_name;
			$this->label = $label;
			$this->type = $type;
			$this->default_value = $default_value;
		}// end function
		
		function getFlagName(){			return $this->flag_name;		}		
		function getLabel(){			return $this->label;		}
		function getType(){			return $this->type;		}
		function getValue(){			return $this->default_value;		}
	}// end class attribute_flags
	
	// container class for source relations
	class relation_source{
          var $source_table;
          var $destination_array;
          var $current_location;
          var $primary_key;

          function __construct($source_table, $primary_key=""){
			$this->source_table = $source_table;
			$this->destination_array = array();
			$this->current_location = 0;
			$this->primary_key = $primary_key;
	   }// end function

          function relation_source($source_table, $primary_key=""){
               $this->source_table = $source_table;
               $this->destination_array = array();
               $this->current_location = 0;
               $this->primary_key = $primary_key;
          }// end function
          
          function getPrimaryKey(){return $this->primary_key;}
          function getTableName(){return $this->source_table;}
          function addDestination($destination_table, $destination_field, $source_field, $relation_type){
               // check if destination not already added
               if(!$this->exists($destination_table, $destination_field, $source_field, $relation_type)){
                    $this->destination_array[] = new relation_destination($destination_table, $destination_field, $source_field, $relation_type);
               }// end if               
          }// end function
          
          // check if exact destination already exists
          function exists($destination_table, $destination_field, $source_field, $relation_type){
               $total_items = count($this->destination_array);
               if($total_items < 1){return FALSE;}
               else{
                    for($i=0; $i < $total_items; $i++){
                         $relation_destination = $this->destination_array[$i];
                         if(  $relation_destination->getDestinationTable() == $destination_table &&
                              $relation_destination->getDestinationField() == $destination_field &&
                              $relation_destination->getSourceField() == $source_field &&
                              $relation_destination->getRelationType() == $relation_type)
                         {return true;}
                    }// end for
                    return FALSE;
               }
          }// end function
          
          function getDestination($location=0){
               $total_items = count($this->destination_array);
               if($location < $total_items){
                    $this->current_location = $location;
                    return $this->destination_array[$location];
               }else{return FALSE;}
          }// end function
          
          function getNextDestination(){
               $total_items = count($this->destination_array);
               if($this->current_location < $total_items){
                    $dobject = $this->destination_array[$this->current_location];
                    $this->current_location++;
                    return $dobject;
               }else{return FALSE;}
          }// end function          
     }// end class relation_source
     
     // container class for destination relations
     class relation_destination{
        var $destination_table;
        var $destination_field;
        var $source_field;
        var $relation_type; // o2m (one to many), m2o (many to one)

        function __construct($destination_table, $destination_field, $source_field, $relation_type){
			$this->destination_table = $destination_table;
			$this->destination_field = $destination_field;
			$this->source_field = $source_field;
			$this->relation_type = $relation_type;
		  }// end function
		  
		
        function relation_destination($destination_table, $destination_field, $source_field, $relation_type){
          $this->destination_table = $destination_table;
          $this->destination_field = $destination_field;
          $this->source_field = $source_field;
          $this->relation_type = $relation_type;
        }// end function
        
        function getDestinationTable(){ return $this->destination_table;}
        function getDestinationField(){ return $this->destination_field;}
        function getSourceField(){ return $this->source_field;}
        function getRelationType(){ return $this->relation_type;}
     }// end class relation_destination
?>
