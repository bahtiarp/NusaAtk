<?php
  /**
   * @package atk
   * @subpackage attributes
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 1.01 $
   * $Id: class.atkorderattribute.inc,v 1.01 2005/10/23 16:28:00 rene Exp $
   */  


  /**  
   * The atkOrderAttribute class represents an attribute of an atkNode.
   * atkOrderAttribute shows a special form where a user can visualy change the order of fields
   * this attribute needs two columns in your table
   * the column with the order (int)
   * the column with descriptions (varchar)
   * 
   * the new value stored in the order column is a integer based on the recordloop index+1 * 5 ($i+1*5)
   * so if you insert a new value in the table you can leave the sort column to 0 and this attribute will sort it out for you.
   * 
   * @author Rene Bakx <rene@dutchphotozone.com>
   * @package atk
   * @subpackage attributes
   *
   */

   /*
     Updates:
     Jan 3, 2007
     * Jorge Garifuna <info@GariDigital.com>
     - changed $inputArray[$i]["cat_order"] to $inputArray[$i][$this->m_sort_column] on edit()
     - changed $inputArray[$i]["cat_title"] to $inputArray[$i][$this->m_desc_column] on edit()
     - changed /|\ to &uarr; (?)
     - changed \|/ to &darr; (?)

   */
  class atkOrderAttribute extends atkAttribute
  {
     // the column with the description
     var $m_desc_column;
     // the order column
     var $m_sort_column;
     // the new order
     var $m_new_sort_column;
     // the new order
     var $m_old_sort_column;
     // query filter
     var $m_filter;
    /**
     * Constructor
     *
     * <b>Example:</b>
     * $this->add(new atkOrderAttribute("naam","desc","order","filter",FLAGS));
     * @param $name Name of the attribute
     * @param $desc Column with the descriptions
     * @param $order Column with the order
     * @param $filter SQL filter (where statement) 
     * @param $flags Flags for the attribute
     
     */

    function atkOrderAttribute($name ,$m_desc_column,$m_sort_column,$m_filter='',$flags = 0)
    {
     $this->m_desc_column = $m_desc_column;
     $this->m_sort_column = $m_sort_column;
     $this->m_filter      = $m_filter;
    }

    function fetchValue($postvars)
    {
  
     $this->m_new_sort_column = $postvars["sectionOrder"];
     $this->m_old_sort_column = $postvars["currentOrder"];
    } 

    function edit($record="", $fieldprefix="")
    {
     
      global $config_atkroot;
      $db   =  &atkGetDb();
      $id   = $this->getHtmlId($fieldprefix);  
      $page = &atkPage::getInstance();
      
      $page->register_script($config_atkroot."atk/javascript/changeorder.js");      
      
      $qry = "SELECT ".$this->m_desc_column.",".$this->m_sort_column."
              FROM ".$this->m_ownerInstance->m_table;
              
      if ($this->m_filter !="")
          {
           $qry .=" WHERE ".$this->m_filter." ";
          }
      $qry .=" ORDER BY ".$this->m_sort_column;                                
      $inputArray = $db->getrows($qry);
      atk_var_dump($inputArray);
      for ($i=0;$i<count($inputArray);$i++)
      {
        // $optionList .= '<option value="'.$inputArray[$i]["cat_order"].'">'.$inputArray[$i]["cat_title"].'</option>'; // commented by Jorge Garifuna on Jan 3, 2007
        $optionList .= '<option value="'.$inputArray[$i][$this->m_sort_column].'">'.$inputArray[$i][$this->m_desc_column].'</option>';
        // $currentOrder .=  $inputArray[$i]["cat_order"]; // commented by Jorge Garifuna on Jan 3, 2007
        $currentOrder .=  $inputArray[$i][$this->m_sort_column];
        
 	   if ($i<count($inputArray)-1) {$currentOrder .=",";}
      }
      
      $form = '<select style="width:400px;" id="selectb" name="selectb" size="'.count($inputArray).'">';
      $form .= $optionList.'</select><br><br>
		<input value="&uarr; up" onclick="move(this.form,this.form.selectb,-1)" type="button">
		<input value="&darr; down" onclick="move(this.form,this.form.selectb,+1)" type="button">
		<input name="sectionOrder" id="sectionOrder" value="'.$currentOrder.'" type="hidden">
		<input name="currentOrder" id="currentOrder" value="'.$currentOrder.'" type="hidden">';
    return $form;
    }
    
       /**
    * VOID implementation.. parserAttribute has no data associated with it, so you can't search it.
    * @param $record Array with fields
    */
    function search($record="", $size=0, $maxsize=0)
    {
      return "&nbsp;";
    }

   /**
    * Display a record
    * Here it will only returns the text.
    * @param $record  Array with fields
    * @return Parsed string
    */
    function display($record)
    {
      return $this->m_text;
    }

    /**
    * Store's our new order in the database
    */
    function store($a, $b)
    {
     $db   =  &atkGetDb();
     $current   = $this->m_new_sort_column;
     $old       = $this->m_old_sort_column;
     $dbName    = $this->m_ownerInstance->m_table;
     $tempOrder = $db->getrows("SELECT ".$this->m_ownerInstance->primaryKeyField().",".$this->m_sort_column." FROM ".$this->m_ownerInstance->m_table);
     
     // create a lookup table for the current order of collumns
     for ($a=0;$a<count($tempOrder);$a++)
     {
       $curOrder[$tempOrder[$a][$this->m_sort_column]] = $tempOrder[$a][$this->m_ownerInstance->primaryKeyField()];
     }
     $newOrder = explode(",",$this->m_new_sort_column);
     $oldOrder  = explode(",",$this->m_old_sort_column);
     unset($updateQuery);
     for($i=0;$i<count($newOrder);$i++)
     {
        $keyId = $curOrder[$newOrder[$i]];
        $newId = $i*10;
        $updateQuery = "UPDATE ".$this->m_ownerInstance->m_table." SET ".$this->m_sort_column."=".$newId
                      ." WHERE ".$this->m_ownerInstance->primaryKeyField()." = ".$keyId;
        $db->query($updateQuery);
     }
     return true;
    }

    /**
    * No function, but is neccesary
    */
    function addToQuery(&$query, $tablename="", $fieldaliasprefix="",$level)
    {
    }

    function getSearchModes()
    {
      // exact match and substring search should be supported by any database.
      // (the LIKE function is ANSI standard SQL, and both substring and wildcard
      // searches can be implemented using LIKE)
      // Possible values
      //"regexp","exact","substring", "wildcard","greaterthan","greaterthanequal","lessthan","lessthanequal"
      return array();
    }
    
    function dbFieldType()
    {
      return "";
    }
    
    function label($record = array())
    {          
      return text($this->m_sort_column,$this->m_ownerInstance->m_type, 
                  $this->m_ownerInstance->m_module);
    }

 } 
?>
