<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage relations
   *
   * @copyright (c)2000-2005 Ibuildings.nl BV
   * @copyright (c)2000-2005 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 6323 $
   * $Id: class.atkmanytomanyrelation.inc 6844 2010-03-31 09:22:41Z peter $
   */
  

  atkimport("atk.relations.atkrelation");
  
  /**
   * Many to many relation. Should not be used directly.
   *
   * This class is used as base class for special kinds of manytomany
   * relations, like the manyboolrelation. Note that most many-to-many
   * relationships can be normalized to a combination of one-to-many and
   * many-to-one relations.
   * 
   * @todo Improve multi-field support. For example setOwnerFields with multiple fields
   *       doesn't work properly at the moment. But it seems more code does not take
   *       multi-field support into account.
   *
   * @abstract
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage relations
   *
   */
  class atkManyToManyRelation extends atkRelation
  {
    var $m_localKey = "";
    var $m_remoteKey = "";
    var $m_link = "";
    var $m_linkInstance = NULL;
    var $m_store_deletion_filter = "";
    var $m_localFilter = NULL;
  
    protected $m_ownerFields = null;
    protected $m_limit;
    
    private $m_selectableRecordsCache = array();
    private $m_selectableRecordCountCache = array();
  
    /**
     * Constructor
     * @param String $name The name of the relation
     * @param String $link The full name of the node that is used as
     *                     intermediairy node. The intermediairy node is
     *                     assumed to have 2 attributes that are named
     *                     after the nodes at both ends of the relation.
     *                     For example, if node 'project' has a M2M relation
     *                     with 'activity', then the intermediairy node
     *                     'project_activity' is assumed to have an attribute
     *                     named 'project' and one that is named 'activity'.
     *                     You can set your own keys by calling setLocalKey()
     *                     and setRemoteKey()
     * @param String $destination The full name of the node that is the other
     *                            end of the relation.
     * @param int $flags Flags for the relation.
     */
    function atkManyToManyRelation($name, $link, $destination, $flags=0)
    {
      $this->m_link = $link;
      $this->atkRelation($name, $destination, $flags|AF_CASCADE_DELETE|AF_NO_SORT);
    }
  
    function __construct($name, $link, $destination, $flags=0)
    {
      $this->m_link = $link;
      $this->atkRelation($name, $destination, $flags|AF_CASCADE_DELETE|AF_NO_SORT);
    }

    /**
     * Returns the selectable records. Checks for an override in the owner instance
     * with name <attribname>_selection.
     *
     * @param array  $record
     * @param string $mode
     * @param bool $force
     * 
     * @return array
     */
    function _getSelectableRecords($record=array(), $mode="", $force=false)
    {
      $method = $this->fieldName()."_selection";
      if (method_exists($this->m_ownerInstance, $method))
        return $this->m_ownerInstance->$method($record, $mode);
      else return $this->getSelectableRecords($record, $mode, $force);
    }
  
    /**
     * Parse destination filter and return the result.
     *
     * @param array $record record
     * 
     * @return string parsed filter
     */
    private function parseDestinationFilter($record)
    {
      $filter = "";
  
      if ($this->m_destinationFilter!="")
      {
        $filter = $this->parseFilter($this->m_destinationFilter, $record);      
      }
  
      return $filter; 
    }
  
    /**
     * Returns the selectable record count.
     *
     * @param array  $record
     * @param string $mode
     * 
     * @return int
     */
    protected function _getSelectableRecordCount($record=array(), $mode="")
    {
      $method = $this->fieldName()."_selection";
      if (method_exists($this->m_ownerInstance, $method))
        return count((array)$this->_getSelectableRecords($record, $mode));
      else return $this->getSelectableRecordCount($record, $mode);
    }
    
    /**
     * Returns the selectable record count. The count is cached unless the 
     * $force parameter is set to true
     *
     * @param array   $record
     * @param string  $mode
     * @param boolean $force 
     * 
     * @return int
     */    
    public function getSelectableRecordCount($record=array(), $mode='', $force=false)
    {
      if (!$this->createDestination())
      {
        return 0;
      }
      
      $filter = $this->parseDestinationFilter($record);

      $cacheKey = md5($filter);
      if (!array_key_exists($cacheKey, $this->m_selectableRecordCountCache) || $force)
      {
        $this->m_selectableRecordCountCache[$cacheKey] = 
          $this->getDestination()
               ->select($filter)
               ->getRowCount();
      }

      return $this->m_selectableRecordCountCache[$cacheKey];
    }    
  
    /**
     * Returns the selectable records for this relation. The records are cached
     * unless the $force parameter is set to true.
     *
     * @param array   $record
     * @param string  $mode
     * @param boolean $force
     * 
     * @return array selectable records
     */
    public function getSelectableRecords($record=array(), $mode="", $force=false)
    {
      if (!$this->createDestination())
      {
        return array();
      }

      $filter = $this->parseDestinationFilter($record);

      $cacheKey = md5($filter);
      if (!array_key_exists($cacheKey, $this->m_selectableRecordsCache) || $force)
      {
        $this->m_selectableRecordsCache[$cacheKey] =
          $this->getDestination()
               ->select($filter)
               ->limit(is_numeric($this->m_limit) ? $this->m_limit : -1)
               ->includes(atk_array_merge($this->m_destInstance->descriptorFields(), $this->m_destInstance->m_primaryKey))
               ->getAllRows();
      }
  
      return $this->m_selectableRecordsCache[$cacheKey];
    }
    
    /**
     * Clears the selectable record count and records cache.
     */
    public function clearSelectableCache()
    {
      $this->m_selectableRecordCountCache = array();
      $this->m_selectableRecordsCache = array();
    }
  
    /**
     * Returns the primary keys of the currently selected records retrieved
     * from the given record.
     *
     * @param array $record current record
     *
     * @return array list of selected record keys
     */
    function getSelectedRecords($record)
    {
      $keys = array();
  
      if (isset($record[$this->fieldName()]))
      {
        for ($i = 0; $i < count((array)$record[$this->fieldName()]); $i++)
        {
          if (is_array($record[$this->fieldName()][$i][$this->getRemoteKey()]))
          {
            $key = $this->m_destInstance->primaryKey($record[$this->fieldName()][$i][$this->getRemoteKey()]);
          }
          else
          {
            $key = $this->m_destInstance->primaryKey(array($this->m_destInstance->primaryKeyField()=>$record[$this->fieldName()][$i][$this->getRemoteKey()]));
          }
  
          $keys[] = $key;
        }
      }
  
      return $keys;
    }
  
    /**
     * Create instance of the intermediairy link node.
     *
     * If succesful, the instance is stored in the m_linkInstance member
     * variable.
     * @return boolean True if successful, false if not.
     */
    function createLink()
    {
      if ($this->m_linkInstance == NULL)
      {
        $this->m_linkInstance = newNode($this->m_link);
  
        // Validate if destination was created succesfully
        if (!is_object($this->m_linkInstance))
        {
          atkerror("Relation with unknown nodetype '".$this->m_link."' (in node '".$this->m_owner."')");
          $this->m_linkInstance = NULL;
          return false;
        }
      }
  
      return true;
    }
    
    /**
     * Returns the link instance. 
     * 
     * The link has to be created first for this method to work.
     * 
     * @return atkNode link instance
     */
    public function getLink()
    {
      return $this->m_linkInstance;
    }
    
  
    /**
     * Get the name of the attribute of the intermediairy node that points
     * to the master node.
     * @return String The name of the attribute.
     */
    function getLocalKey()
    {
      if ($this->m_localKey=="")
      {
        $this->m_localKey = $this->determineKeyName($this->m_owner);
      }
      return $this->m_localKey;
    }
  
    /**
     * Change the name of the attribute of the intermediairy node that points
     * to the master node.
     * @param String $attributename The name of the attribute.
     */
    function setLocalKey($attributename)
    {
      $this->m_localKey = $attributename;
    }
  
    /**
     * Get the name of the attribute of the intermediairy node that points
     * to the node on the other side of the relation.
     * @return String The name of the attribute.
     */
    function getRemoteKey()
    {
      $this->createDestination();
  
      if ($this->m_remoteKey=="")
      {
        list($module, $nodename) = explode(".", $this->m_destination);
        $this->m_remoteKey = $this->determineKeyName($nodename);
      }
      return $this->m_remoteKey;
    }
  
    /**
     * Sets the owner fields in the owner instance. The owner fields are
     * the attribute(s) of the owner instance which map to the local key
     * of the link node.
     *
     * @param array $ownerfields
     */
    public function setOwnerFields($ownerfields)
    {
      $this->m_ownerFields = $ownerfields;
    }
  
    /**
     * Returns the owner fields. The owners fields are the attribute(s)
     * of the owner instance which map to the local key of the link node.
     *
     * @return array owner fields
     */
    public function getOwnerFields()
    {
      if (is_array($this->m_ownerFields) && count((array)$this->m_ownerFields)>0)
      {
        return $this->m_ownerFields;
      }
      return $this->m_ownerInstance->m_primaryKey;
    }
  
    /**
     * Determine the name of the foreign key based on the name of the
     *  relation.
     *
     * @param String $name the name of the relation
     * @return the probable name of the foreign key
     */
    function determineKeyName($name)
    {
      if ($this->createLink())
      {
        if (isset($this->m_linkInstance->m_attribList[$name]))
        {
          // there's an attribute with the same name as the role.
          return $name;
        }
        else
        {
          // find out if there's a field with the same name with _id appended to it
          if (isset($this->m_linkInstance->m_attribList[$name."_id"]))
          {
            return $name."_id";
          }
        }
      }
      return $name;
    }
  
    /**
     * Change the name of the attribute of the intermediairy node that points
     * to the node on the other side of the relation.
     * @param String $attributename The name of the attribute.
     */
    function setRemoteKey($attributename)
    {
      $this->m_remoteKey = $attributename;
    }
  
    /**
     * Returns a displayable string for this value.
     * 
     * @param array $record The record that holds the value for this attribute
     * @param String $mode The display mode ("view" for viewpages, or "list"
     *                     for displaying in recordlists, "edit" for
     *                     displaying in editscreens, "add" for displaying in
     *                     add screens. "csv" for csv files. Applications can
     *                     use additional modes.
     * @return a displayable string for this value
     */
    function display($record, $mode="")
    {
      if (!in_array($mode, array("csv", "plain"))) { $result = "&nbsp;"; } else { $result=''; }
        if ($this->createDestination() && atk_value_in_array($record[$this->fieldName()]))
        {
          $recordset = array();
          $remotekey = $this->getRemoteKey();
          for ($i=0;$i<count((array)$record[$this->fieldName()]);$i++)
          {
            if(!is_array($record[$this->fieldName()][$i][$remotekey]))
            {
              $selector = $this->m_destInstance->m_table.".".$this->m_destInstance->primaryKeyField()."='".$record[$this->fieldName()][$i][$remotekey]."'";
              list($rec) = $this->m_destInstance->selectDb($selector,"","","",$this->m_destInstance->descriptorFields());
              $recordset[] = $this->m_destInstance->descriptor($rec);
            }
            else
            {
              $recordset[] = $this->m_destInstance->descriptor($record[$this->fieldName()][$i][$remotekey]);
            }
          }
          if (!in_array($mode, array("csv", "plain")))
          {
            $result = "<ul><li>".implode("<li>",$recordset)."</ul>";
          }
          else
          {
            $result = implode(", ",$recordset);
          }
        }
        return $result;
    }
  
    /**
     * Dummy function
     * 
     * @param array $record The record that holds the value for this attribute.
     * @param String $fieldprefix The fieldprefix to put in front of the name
     *                            of any html form element for this attribute.
     * @param String $mode The mode we're in ('add' or 'edit')
     * @return String A piece of htmlcode for editing this attribute
     */
    function edit($record="", $fieldprefix="", $mode="")
    {
    }
  
    /**
     * Dummy function (we don't add ourselves to the query)
     * @param atkQuery $query The SQL query object
     * @param String $tablename The name of the table of this attribute
     * @param String $fieldaliasprefix Prefix to use in front of the alias
     *                                 in the query.
     * @param Array $rec The record that contains the value of this attribute.
     * @param int $level Recursion level if relations point to eachother, an
     *                   endless loop could occur if they keep loading
     *                   eachothers data. The $level is used to detect this
     *                   loop. If overriden in a derived class, any subcall to
     *                   an addToQuery method should pass the $level+1.
     * @param String $mode Indicates what kind of query is being processing:
     *                     This can be any action performed on a node (edit,
     *                     add, etc) Mind you that "add" and "update" are the
     *                     actions that store something in the database,
     *                     whereas the rest are probably select queries.
     */
    //function addToQuery(&$query, $tablename="", $fieldaliasprefix="", $rec, $level, $mode)
    function addToQuery(&$query, $tablename, $fieldaliasprefix, &$rec, $level, $mode)     
    {
      // we don't add ourselves to the query;
    }
  
    /**
     * load function
     * @param atkDb $notused
     * @param array $record
     */
    function load(atkDb $db, $record, $mode)
    {
      if ($this->createLink())
      {
        $where = $this->_getLoadWhereClause($record);
        $rel = $this->m_linkInstance;
        return $rel->selectDb($where);
      }
      return array();
    }
  
    /**
     * Get where clause for loading the record
     *
     * @param array $record The record
     * @return string The where clause
     */
    function _getLoadWhereClause($record)
    {
      $whereelems = array();
      $localkey = $this->getLocalKey();
      if(!is_array($localkey)) $localkey = array($localkey);
  
      $ownerfields = $this->getOwnerFields();
  
      for ($i=0, $_i = count((array)$localkey); $i<$_i; $i++)
      {
        $primkeyattr = $this->m_ownerInstance->m_attribList[$ownerfields[$i]];
  
        if (!$primkeyattr->isEmpty($record))
        {
          $whereelems[] = $this->m_linkInstance->m_table.".".$localkey[$i]."='".$primkeyattr->value2db($record)."'";
        }
      }
  
      if ($this->m_localFilter != NULL)
        $whereelems[] = $this->m_localFilter;
  
      return "(".implode(") AND (", $whereelems).")";
    }
  
    /**
     * delete relational records..
     * 
     * @param array $record The record
     */
    function delete($record)
    {
      if ($this->createLink())
      {
        $rel = $this->m_linkInstance;
        $where = $this->_getLoadWhereClause($record);
        if($where!='')
          return $rel->deleteDb($where);
      }
      return false;
    }
  
    /**
     * Returns an array with the existing records indexed by their 
     * primary key selector string.
     * 
     * @param atkDb  $db      database instance
     * @param array  $record  record
     * @param string $mode    mode
     */
    protected function _getExistingRecordsByKey(atkDb $db, $record, $mode)
    {
      $existingRecords = $this->load($db, $record, $mode);
      $existingRecordsByKey = array();
      foreach ($existingRecords as $existingRecord)
      {
        $existingRecordKey = 
          is_array($existingRecord[$this->getRemoteKey()]) ? 
          $existingRecord[$this->getRemoteKey()][$this->getDestination()->primaryKeyField()] :
          $existingRecord[$this->getRemoteKey()];
  
        $existingRecordsByKey[$existingRecordKey] = $existingRecord;
      }
  
      return $existingRecordsByKey;      
    }
  
    /**
     * Extracts the selected records from the owner instance record for
     * this relation and index them by their primary key selector string.
     * 
     * @param array $record record
     */
    protected function _extractSelectedRecordsByKey($record)
    {
      $selectedRecordsByKey = array();
      foreach ($record[$this->fieldName()] as $selectedRecord)
      {
        $selectedKey = 
          is_array($selectedRecord[$this->getRemoteKey()]) ?
          $selectedRecord[$this->getRemoteKey()][$this->getDestination()->primaryKeyField()] :
          $selectedRecord[$this->getRemoteKey()];
  
        $selectedRecordsByKey[$selectedKey] = $selectedRecord;
      }
  
      return $selectedRecordsByKey;      
    }
  
    /**
     * Delete existing link record.
     * 
     * @param array $record link record
     */
    protected function _deleteRecord($record)
    {
      $selector = $this->getLink()->primaryKey($record);
  
      // append the store deletion filter (if set)
      if (!empty($this->m_store_deletion_filter))
      {
        $selector = "({$selector}) AND ({$this->m_store_deletion_filter})";
      }
  
      return $this->getLink()->deleteDb($selector);
    }
  
    /**
     * Update existing link record.
     * 
     * @param array $record link record
     * @param int   $index  (new) index (0-based)
     */
    protected function _updateRecord($record, $index)
    {
      // don't do anything by default
      return true;
    }
  
    /**
     * Create new link record.
     * 
     * @param string $selectedKey     primary key selector string of destination record
     * @param array  $selectedRecord  selected destination record (might only contain the key attributes)
     * @param array  $ownerRecord     owner instance record
     * @param int    $index           (new) index (0-based)
     * 
     * @return array new link record (not saved yet!)
     */
    protected function _createRecord($selectedKey, $selectedRecord, $ownerRecord, $index)
    {
      $record = array_merge($this->getLink()->initial_values(), $selectedRecord);
      $record[$this->getRemoteKey()] = $selectedKey;
  
      $ownerFields = $this->getOwnerFields();
      $localKey = $this->getLocalKey();
  
      if (is_array($localKey))
      {
        for ($j = 0; $j < count((array)$localKey); $j++)
        {
          $locKey = $this->checkKeyDimension($ownerRecord[$ownerFields[$j]]);
          $record[$localKey[0]][$ownerFields[$j]] = $locKey;
        }
      }
      else
      {
        $locKey = $this->checkKeyDimension($ownerRecord[$ownerFields[0]]);
        $record[$localKey] = $locKey;
      }  
  
      return $record;    
    }
  
    /**
     * Add new link record to the database.
     * 
     * @param array  $record link record
     * @param int    $index  (new) index (0-based)
     * @param string $mode  storage mode
     */
    protected function _addRecord($record, $index, $mode)
    {
      return $this->getLink()->addDb($record, true, $mode);
    }
  
    /**
     * Stores the values in the database
     * 
     * @param atkDb  $db     database instance
     * @param array  $record owner instance record
     * @param string $mode   storage mode
     */
    function store($db, $record, $mode)
    {
      $this->createLink();
      $this->createDestination();
  
      $existingRecordsByKey = $this->_getExistingRecordsByKey($db, $record, $mode);
      $existingRecordsKeys = array_keys($existingRecordsByKey);
  
      $selectedRecordsByKey = $this->_extractSelectedRecordsByKey($record);
      $selectedRecordsKeys = array_keys($selectedRecordsByKey);

      // first delete the existing records that aren't selected anymore
      $deleteKeys = array_diff($existingRecordsKeys, $selectedRecordsKeys);
  
      foreach ($deleteKeys as $deleteKey)
      {
        if (!$this->_deleteRecord($existingRecordsByKey[$deleteKey]))
        {
          return false;
        }
      }
  
      // then add new or update existing records
      $index = 0;
      foreach ($selectedRecordsByKey as $selectedKey => $selectedRecord)
      {
        if (isset($existingRecordsByKey[$selectedKey]))
        {
          if (!$this->_updateRecord($existingRecordsByKey[$selectedKey], $index))
          {
            return false;
          }
        }
        else
        {
          $newRecord = $this->_createRecord($selectedKey, $selectedRecord, $record, $index);
  
          if (!$this->_addRecord($newRecord, $index, $mode))
          {
            return false;
          }
        }
  
        $index++;
      }   
  
      return true;    
    } 
  
    /**
     * Check if the attribute is empty
     * 
     * @param array $postvars
     * @return true if it's empty
     */
    function isEmpty($postvars)
    {
      return (!is_array($postvars[$this->fieldName()]) || count((array)$postvars[$this->fieldName()])==0);
    }
  
    /**
     * Returns a piece of html code for hiding this attribute in an HTML form,
     * while still posting its value. (<input type="hidden">)
     *
     * @param array $record The record that holds the value for this attribute
     * @param String $fieldprefix The fieldprefix to put in front of the name
     *                            of any html form element for this attribute.
     * @return String A piece of htmlcode with hidden form elements that post
     *                This attribute's value without showing it.
     */
    function hide($record="", $fieldprefix="")
    {
      $result = "";
      if(is_array(atkArrayNvl($record,$this->fieldName())) && $this->createDestination())
      {
        $ownerFields = $this->getOwnerFields();
        for($i=0,$_i=count((array)$record[$this->fieldName()]);$i<$_i;$i++)
        {
          if(atkArrayNvl($record[$this->fieldName()][$i],$this->getLocalKey()))
            $result .= '<input type="hidden" name="'.$fieldprefix.$this->formName().
            '['.$i.']['.$this->getLocalKey().']" value="'.
            $this->checkKeyDimension($record[$this->fieldName()][$i][$this->getLocalKey()],
              $ownerFields[0]).'">';
  
          if(atkArrayNvl($record[$this->fieldName()][$i],$this->getRemoteKey()))
            $result .= '<input type="hidden" name="'.$fieldprefix.$this->formName().
            '['.$i.']['.$this->getRemoteKey().']" value="'.
            $this->checkKeyDimension($record[$this->fieldName()][$i][$this->getRemoteKey()],
              $this->m_destInstance->primaryKeyField()).'">';
        }
      }
      return $result;
    }
  
    /**
     * Returns a piece of html code that can be used in a form to search
     * 
     * @param array $record Array with values
     * @param boolean $extended if set to false, a simple search input is
     *                          returned for use in the searchbar of the
     *                          recordlist. If set to true, a more extended
     *                          search may be returned for the 'extended'
     *                          search page. The atkAttribute does not
     *                          make a difference for $extended is true, but
     *                          derived attributes may reimplement this.
     * @param string $fieldprefix The fieldprefix of this attribute's HTML element.
     *
     * @return string Piece of html code
     */
    //function search($record = Array, $extended = false, $fieldprefix = '',  $grid = NULL)
    function search($record="", $extended=false, $fieldprefix="")
    {
      $this->createDestination();
  
      // now select all records
      $recordset = $this->m_destInstance->selectDb("","","","*",atk_array_merge($this->m_destInstance->descriptorFields(),$this->m_destInstance->m_primaryKey));
      $result = '<select ';
      if ($extended)
      {
        $result.='multiple="multiple" size="'.min(5,count((array)$recordset)+1).'"';
      }
  
      $result.='name="'.$this->getSearchFieldName($fieldprefix).'[]">';
  
      $pkfield = $this->m_destInstance->primaryKeyField();
  
      $result.= '<option value="">'.atktext("search_all", "atk").'</option>';
  
      for ($i=0;$i<count((array)$recordset);$i++)
      {
        $pk = $recordset[$i][$pkfield];
        if (atk_in_array($pk, $record[$this->fieldName()])) $sel = ' selected="selected"'; else $sel = "";
        $result.= '<option value="'.$pk.'"'.$sel.'>'.$this->m_destInstance->descriptor($recordset[$i]).'</option>';
      }
      $result.='</select>';
      return $result;
    }
  
    /**
     * Creates an search condition for a given search value
     * 
     * @param atkQuery $query The query to which the condition will be added.
     * @param String $table The name of the table in which this attribute
     *                      is stored
     * @param mixed $value The value the user has entered in the searchbox
     * @param String $searchmode The searchmode to use. This can be any one
     *                           of the supported modes, as returned by this
     *                           attribute's getSearchModes() method.
     * @param string $fieldaliasprefix optional prefix for the fieldalias in the table
     */
    function searchCondition(&$query, $table, $value, $searchmode, $fieldaliasprefix='')
    {
      $ownerFields = $this->getOwnerFields();
  
      // We only support 'exact' matches.
      // But you can select more than one value, which we search using the IN() statement,
      // which should work in any ansi compatible database.
      if (is_array($value) && count((array)$value)>0 && $value[0]!="") // This last condition is for when the user selected the 'search all' option, in which case, we don't add conditions at all.
      {
        $this->createLink();
        $query->addJoin($this->m_linkInstance->m_table, $this->fieldName(), $table.".".$ownerFields[0]."=".$this->fieldName().".".$this->getLocalKey(),FALSE );
        $query->setDistinct(TRUE);
  
        if (count((array)$value)==1) // exactly one value
        {
          $query->addSearchCondition($query->exactCondition($this->fieldName().".".$this->getRemoteKey(),$this->escapeSQL($value[0])));
        }
        else // search for more values using IN()
        {
          $query->addSearchCondition($this->fieldName().".".$this->getRemoteKey()." IN ('".implode("','",$value)."')");
        }
      }
    }
  
    /**
     * Checks if a key is not an array
     * @param string $key   field containing the key values
     * @param string $field field to return if an array
     * @return value of $field
     */
    function checkKeyDimension($key, $field="id")
    {
      if (is_array($key))
      {
        return $key[$field];
      }
      return $key;
    }
  
    /**
     * Fetch value. If nothing selected, return empty array instead
     * of nothing.
     *
     * @param array $postvars
     */
    function fetchValue($postvars)
    {
      $value = parent::fetchValue($postvars);
      return $value == NULL ? array() : $value;
    }
  
    /**
     * Function adds a custom filter that is used when deleting items during the store() function.
     *
     * Example:
     * Normally the delete function would do something like this:
     *
     * DELETE FROM phase WHERE phase.template NOT IN (1,2,3)
     *
     * If the template field is NULL, although it is not specified in the NOT IN (1,2,3), it will not be deleted.
     * An extra check can be added just in case the template value is not NULL but 0 or '' (which would delete the phase).
     *
     * @param String $filter The filter that is used when deleting records in the store function.
     * @return none
     */
    function setStoreDeletionFilter($filter)
    {
      $this->m_store_deletion_filter = $filter;
    }
  
    /**
     * Local filter is used to only show values that are once selected
     * that comply with the local filter. A local filter is also automatically
     * set as store deletion filter.
     *
     * @param string $filter filter
     */
    function setLocalFilter($filter)
    {
      $this->setStoreDeletionFilter($filter);
      $this->m_localFilter = $filter;
    }
  }
?>
