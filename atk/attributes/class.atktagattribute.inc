<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage attributes
   *
   * @copyright (c)2006 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 6305 $
   * $Id: class.atktagattribute.inc 6410 2009-06-25 20:36:10Z sandy $
   */

  /**
   * Flags for use in atkTagAttribute constructor
   */
  define("TA_ADD",    1);//When a none-existing tag was found, the tag is added to the defaults.
  define("TA_ERROR",  2);//When a none-existing tag was found, an error is triggered.
  define("TA_IGNORE", 3);//When a none-existing tag is found, the tag is ignored.

  /**
   * @internal base include
   */
  atkimport("atk.attributes.atkfuzzysearchattribute");
  atkimport("atk.attributes.atktextattribute");

  /**
   * This attribute is used for adding tags to a node
   * For example: This attribute can be used in a blog node
   * for adding tags to the blogpost.
   *
   * @author Dennis Luitwieler <dennis@ibuildings.nl>
   * @package atk
   * @subpackage attributes
   */
  class atkTagAttribute extends atkFuzzySearchAttribute
  {
    var $m_link = "";
    var $m_linkInstance = NULL;
    var $m_destination ="";
    var $m_destInstance = NULL;
    var $m_destinationfield = "";
    var $m_remoteKey = "";
    var $m_localKey = "";
    var $m_nonematching = array();

    /**
     * Constructor
     *
     * @param string $name
     * @param string $destination
     * @param string $destinationfield
     * @param string $link
     * @param int $mode
     * @param int $flags
     * @param int $size
     * @return atkTagAttribute
     */
    function atkTagAttribute($name, $destination, $destinationfield ,$link, $mode=TA_ADD, $flags=0, $size=0)
    {
      if ($size==0)
      {
        $size = $this->maxInputSize();
      }

      $this->atkAttribute($name, $flags|AF_NO_SORT, $size);
      $this->m_destination = $destination;
      $this->m_destinationfield = $destinationfield;
      $this->m_link = $link;
      $this->m_mode = $mode;

      /** @todo: if not allowed, only show the tags as text and not as an inputfield */
      /** @todo: add translations */
      /** @todo: add validation if flag OBLIGATORY is set. */
      /** @todo: add setDestinationFilter */
    }
    
    /**
     * Fetch value.
     * 
     * @param array $vars post vars
     * 
     * @return string fetched value
     */
    public function fetchValue($vars)
    {
      $value = parent::fetchValue($vars);
      return trim($value, ' ,');
    }

    /**
     * Creates an instance of the node we are searching on and stores it
     * in a member variable ($this->m_destInstance)
     * @return bool True if successful, false if not.
     */
    function createDestinationInstance()
    {
      if (!is_object($this->m_destInstance))
      {
        $this->m_destInstance = &getNode($this->m_destination);
        return is_object($this->m_destInstance);
      }
      return true;
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
      if (!is_object($this->m_linkInstance))
      {
        $this->m_linkInstance = &getNode($this->m_link);
        return is_object($this->m_linkInstance);
      }
      return true;
    }


    /**
     * Validate the input based on the current mode.
     *
     * @param Array $rec The record which holds the values to validate
     * @param String $mode The mode we're in
     * @return True on validation, False otherwise.
     */
    function validate(&$rec, $mode)
    {
      $valid = true;
      
      // If coming from selectscreen, no search necessary anymore.
      if (is_array($rec[$this->fieldName()]))      
        return true;      
      
      $this->m_matches = $this->getMatches($rec[$this->fieldName()]);
      $this->m_nonematching = $this->getNoneMatching();

      foreach ($this->m_nonematching as $keyword)
      {
        if ($this->m_mode == TA_ERROR)
        {
          atkTriggerError($rec,$this,"notallowed_new_defaulttag",sprintf($this->text("notallowed_new_defaulttag"),$keyword));
          $valid = false;
        }
        elseif($this->m_mode == TA_ADD && !$this->isValidKeyWord($keyword))
        {
          atkTriggerError($rec, $this, 'error_tag_illegalvalue', sprintf($this->text('error_tag_illegalvalue'), $keyword));
          $valid = false;
        }
      }
    
      return $valid;
    }
    
    /**
     * A keyword field may not contain HTML or linefeeds
     *     
     * @param String $keyword
     */
    function isValidKeyWord($keyword)
    {
      $replaced = strip_tags(str_replace("\n",'', str_replace("\r\n",'',$keyword)));      
      return (atk_strlen($keyword) == atk_strlen($replaced));    
    }

    /**
     * Returns a piece of html code that can be used in a form to edit this
     * attribute's value.
     *
     * @param array $rec The record that holds the value for this attribute.
     * @param String $prefix The fieldprefix to put in front of the name
     *                            of any html form element for this attribute.
     * @param String $mode The mode we're in ('add' or 'edit')
     * @return String A piece of htmlcode for editing this attribute
     */
    function edit($rec="", $prefix="", $mode="")
    {
      atkdebug("edit of attribute '$this->fieldName()'");
      
      $page = atkinstance('atk.ui.atkpage');
      $page->register_script(atkconfig("atkroot")."atk/javascript/class.atktagattribute.js");

      if ($this->createDestinationInstance())
      {
        $html = $this->displayDefaultTags($prefix);

        //only refill the record, if we are not in TA_ERROR mode, and no errors are found.
        if(!(count((array)$this->m_nonematching) && $this->m_mode==TA_ERROR))
          $rec[$this->fieldName()] = $this->refillRecord($rec);

        $html .= atkTextAttribute::edit($rec, $prefix, $mode);
        return $html;
      }
      else
      {
        atkdebug("could not create destination instance");
        return false;
      }
    }

    /**
     * Display default tags
     *
     * @param string $prefix
     * @return string The HTML code to display the default tags
     */
    function displayDefaultTags($prefix='')
    {
      $id = $this->getHtmlid($prefix);

      $defaults = $this->_getDefaultTags();

      $html = '<div id="'.$this->fieldName().'_tags">';

      if(count((array)$defaults))
      {
        $html.= $this->text("available_default_tags").": ";
        for($i=0,$_i=count((array)$defaults);$i<$_i;$i++)
        {
          $key = $defaults[$i][$this->m_destinationfield];

          $jsclick = "ta_addTag('$id', '$key');";

          $html.= "<a href=\"javascript:$jsclick\">$key</a>";

          if($i<($_i-1))
            $html.= ", ";
        }
      }
      else
      {
        return '';
      }

      $html.= "</div>";
      return $html;
    }

    /**
     * Refill record
     *
     * @param array $orgrec
     * @return string
     */
    function refillRecord($orgrec)
    {
      if(!$this->createLink())
        return "";

      $values = array();

      $objectid = $orgrec[$this->m_ownerInstance->primaryKeyField()]; //pageid or topicid
      $selector = $this->m_linkInstance->m_table.".".$this->getLocalKey()."='".$objectid."'";
      $records = $this->m_linkInstance->selectDb($selector);

      //loop through all the records
      foreach($records as $r)
      {
        $tagname = $r[$this->getRemoteKey()][$this->m_destinationfield];
        $tagname = trim($tagname);

        //we do not want duplicates
        if(!in_array($tagname,$values))
          $values[] = $tagname;

      }

      return implode(", ",$values);
    }

    /**
     * Returns a displayable string for this value, to be used in HTML pages.
     *
     * @param array $record The record that holds the value for this attribute
     * @param String $mode The display mode ("view" for viewpages, or "list"
     *                     for displaying in recordlists, "edit" for
     *                     displaying in editscreens, "add" for displaying in
     *                     add screens. "csv" for csv files. Applications can
     *                     use additional modes.
     * @return String HTML String
     */
    function display($record, $mode="")
    {
      if (is_array($record[$this->fieldName()]))
      {
        return $this->arrayToString($record[$this->fieldName()]);
      }

      return nl2br(htmlspecialchars($record[$this->fieldName()]));
    }

    /**
     * Convert array to string
     *
     * @param array $array The array to convert
     * @return string String representation of the array
     */
    function arrayToString($array)
    {
      $values=array();
      foreach($array as $a)
      {
        $values[] = nl2br(htmlspecialchars($a[$this->getRemoteKey()][$this->m_destinationfield]));
      }
      return implode(", ",$values);
    }
    
    /**
     * The actual function that does the searching
     * @param String $searchstring The string to search for
     * @return Array The matches
     */
    function getMatches($searchstring)
    {
      atkdebug("Performing search");
      $result = array();

      if ($this->createDestinationInstance() && $searchstring!="")
      {
        $tokens = explode(",", $searchstring);
        foreach ($tokens as $token)
        {
          $token = trim($token);
          if($token!="")
          {
            $result[$token] = $this->getDestinationRecords($token);            
          }
        }
      }
      return $result;
    }
    
    /**
     * Get destination records
     *
     * @param mixed $token The value of the destinationfield to select
     * @return array Array with records that match the token
     */
    function getDestinationRecords($token)
    {
      $selector = $this->m_destInstance->m_table.".".$this->m_destinationfield."='".escapeSQL($token)."'";
      return $this->m_destInstance->selectDb($selector);
    }
    
    /**
     * Get the keywords that did not match. 
     *
     * @return array
     */
    function getNoneMatching()
    {      
      $no_match = array();
      
      //loop through all the matches for each keyword
      foreach($this->m_matches as $keyword=>$matches)
      {
        $cnt_matches = count((array)$matches);
        //We remember the none-matching ones.
        if(!$cnt_matches) $no_match[] = $keyword;
      }
      return $no_match;
    }

    /**
     * Get default tags from the database
     *
     * @return array Array with default tag records
     */
    function _getDefaultTags()
    {
      if($this->createDestinationInstance())
      {
        return $this->m_destInstance->selectDb();
      }
      return array();
    }

    /**
     * Store the value of this attribute
     *
     * @param atkDb $db The database object
     * @param array $rec The record to store
     * @param string $mode The mode we're in
     * @return bool True if succesfull, false if not
     */
    function store($db, $rec, $mode)
    {
      $resultset = array();

      if(!$this->createLink())
      {
        atkdebug("could not create an instance for the link '$this->m_link'");
        return false;
      }

      if(!$this->createDestinationInstance())
      {
        atkdebug("could not create an instance for the destination '$this->m_destination'");
        return false;
      }

      //first delete all old tags for this object.
      $objectid = $rec[$this->m_ownerInstance->primaryKeyField()]; //pageid or topicid
      $selector = $this->m_linkInstance->m_table.".".$this->getLocalKey()."='".$objectid."'";

      if(!$this->m_linkInstance->deleteDb($selector))
      {
        atkdebug("could not delete the linked default tags");
        return false;
      }

      $no_match = array();

      //loop through all the matches for each keyword
      foreach($this->m_matches as $keyword=>$matches)
      {
        $cnt_matches = count((array)$matches);

        //We remember the none-matching ones.
        if(!$cnt_matches) $no_match[] = $keyword;

        for ($i=0; $i<$cnt_matches; $i++)
        {
          // Make sure there are no duplicates
          if (!in_array($matches[$i], $resultset))
          {
            $resultset[] = $matches[$i];
          }
        }
      }

      //if we are in the TA_ADD mode, we add the none-matching keywords.
      if(count((array)$no_match) && $this->m_mode==TA_ADD)
      {
        // add default keyword
        foreach($no_match as $keyword)
        {
          $defaultsRec[$this->m_destinationfield] = $keyword;
          
          
          //if one keyword could not be added, stop adding them.
          if(!$this->m_destInstance->validate($defaultsRec, 'add') || !$this->m_destInstance->addDb($defaultsRec))
          {
            atkdebug("could not add default keyword");
            return false;
          }
          else
          {
            $newrecord = decodeKeyValueSet($defaultsRec["atkprimkey"]);
            $newrecord[$this->m_destinationfield] = $keyword;
            $newrecord["atkprimkey"]              = $defaultsRec["atkprimkey"];

            //add newly added record to the resultset
            $resultset[] = $defaultsRec;
          }
        }
      }

      /** we only support a primary key of one field */
      //store matches
      foreach($resultset as $res)
      {
        $locKey = $rec[$this->m_ownerInstance->primaryKeyField()];
        $remKey = $res[$this->m_destInstance->primaryKeyField()];

        atkdebug("<h2>LOCKEY:$locKey REMKEY:$remKey</h2>");

        $newrecord = $this->m_linkInstance->initial_values();
        $newrecord[$this->getLocalKey()][$this->m_ownerInstance->primaryKeyField()] = $locKey;
        $newrecord[$this->getRemoteKey()][$this->m_destInstance->primaryKeyField()] = $remKey;

        // First check if the record does not exist yet.
        $where = $this->m_linkInstance->m_table.'.'.$this->getLocalKey()."='".
                   $rec[$this->m_ownerInstance->primaryKeyField()]."'".
                   " AND ".$this->m_linkInstance->m_table.'.'.$this->getRemoteKey()."='".
                   $remKey."'";

        $existing = $this->m_linkInstance->selectDb($where, "", "", "", $this->m_linkInstance->m_primaryKey);

        if (!count((array)$existing))
        {
          atkdebug("does not exist, adding new record.");

          if (!$this->m_linkInstance->addDb($newrecord, true, $mode))
          {
            atkdebug("could not add keyword");
            return false;
          }
        }
      }

      return true;
    }   
    
    /**
     * load function
     * @param atkDb $notused
     * @param array $record
     */
    function load($notused, $record)
    {
      atkdebug("calling load");
      if ($this->createLink())
      {
        return $this->m_linkInstance->selectDb($this->m_linkInstance->m_table.".".$this->getLocalKey()."='".$record[$this->m_ownerInstance->primaryKeyField()]."'");
      }
      return array();
    }

    /**
     * Dummy implementation
     *
     */
    function addToQuery() {}
    
    /**
     * Dummy implementation
     *
     */
    function hide() {}
    
    /**
     * Dummy implementation
     *
     */
    function search() {}
    
    /**
     * Dummy implementation
     *
     */
    function getSearchModes() { return array();}
    
    /**
     * Dummy implementation
     *
     */
    function searchCondition() {}
    
    /**
     * Dummy implementation
     *
     */
    function getSearchCondition() {}
    
    /**
     * Dummy implementation
     *
     */
    function fetchMeta() {}
    
    /**
     * Dummy implementation
     *
     */
    function dbFieldSize() {}
    
    /**
     * Dummy implementation
     *
     */
    function dbFieldType() {}

    /**
     * Determine the maximum length an input field may be.
     * @return int
     */
    function maxInputSize()
    {
      return 70;
    }

    /**
     * Adds a filter on the instance of the destination
     * @param String $filter The fieldname you want to filter OR a SQL where
     *                       clause expression.
     * @param String $value Required value. (Ommit this parameter if you pass
     *                      an SQL expression for $filter.)
     */
    function addSearchFilter($filter, $value="")
    {
      if (!$this->m_destInstance) $this->createdestinationInstance();
      $this->m_destInstance->addFilter($filter, $value);
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
      if ($this->m_remoteKey=="")
      {
        list($module, $nodename) = explode(".", $this->m_destination);
        $this->m_remoteKey = $this->determineKeyName($nodename);
      }
      return $this->m_remoteKey;
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
     * Checks if a key is not an array
     * @param array $key   field containing the key values
     * @param string $field field to return if an array
     * @return mixed value of $field
     */
    function checkKeyDimension($key, $field="id")
    {
      if (is_array($key))
      {
        return $key[$field];
      }
      return $key;
    }
  }
