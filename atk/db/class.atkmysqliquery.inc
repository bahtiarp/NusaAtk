<?php

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage db
   *
   * @copyright (c)2005 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 4930 $
   * $Id: class.atkmysqliquery.inc 6395 2009-06-03 12:32:06Z peter $
   */

 /**
  * @internal Include baseclass
  */
 require_once(atkconfig("atkroot")."atk/db/class.atkmysqlquery.inc");

  /**
   * SQL Builder for MySQL 4.1+ databases.
   *
   * @author Rene van den Ouden <rene@ibuildings.nl>
   * @package atk
   * @subpackage db
   *
   */
  class atkMysqliQuery extends atkMysqlQuery
  {
    /**
     * Reference to the field where the new sequence
     * value should be stored.
     * 
     * @var int
     */
    protected $m_seqValue;
    
    /**
     * Sequence field name.
     * 
     * @var string
     */
    protected $m_seqField;
    
    /**
     * Should we return a sequence value by setting
     * $this->m_seqValue?
     * 
     * @var boolean
     */
    protected $m_returnSeqValue = false;
    
    /**
     * Overriding the _addFrom function to support a change that was made in
     * MySQL 5.0.15 to make MySQL more compliant with the standard.
     *
     * See: http://bugs.mysql.com/bug.php?id=13551
     *
     * @param string $query
     */
    function _addFrom(&$query)
    {
      $query.= " FROM (";
      for ($i=0;$i<count((array)$this->m_tables);$i++)
      {
        $query.= $this->quoteField($this->m_tables[$i]);
        if ($this->m_aliases[$i]!="") $query.=" ".$this->m_aliases[$i];
        if ($i < count((array)$this->m_tables)-1) $query.=", ";
      }
      $query.= ") ";
    }

    /**
     * Builds the SQL Insert query
     * @return String a SQL Insert Query
     */
    function buildInsert()
    {
      
      $result = "INSERT INTO ".$this->quoteField($this->m_tables[0])." (";

      for ($i=0;$i<count((array)$this->m_fields);$i++)
      {
        $result.= $this->quoteField($this->m_fields[$i]);
        if ($i < count((array)$this->m_fields)-1) $result.=",";
      }

      $result.=") VALUES (";

      for ($i=0;$i<count((array)$this->m_fields);$i++)
      {
        if (($this->m_values[$this->m_fields[$i]] === "''") and ($this->m_db->m_tableMeta[$this->m_tables[0]][$this->m_fields[$i]]["type"] == "int")){
          atkdebug("atkMysqliQuery::buildInsert() : '' transformed in '0' for MySQL5 compatibility in field '" . $this->m_fields[$i] . "'");
          $result.= "'0'";
        }else{
          $result.= $this->m_values[$this->m_fields[$i]];
        }
        if ($i < count((array)$this->m_fields)-1) $result.=",";
      }

      $result.=")";

      return $result;
    }
    
    /**
     * Add's a sequence field to the query.
     * 
     * @param string $fieldName field name
     * @param int    $value     field to store the new sequence value in, note certain drivers
     *                          might populate this field only after the insert query has been 
     *                          executed
     * @param string $seqName   sequence name (optional for certain drivers)
     * 
     * @return atkQuery
     */  
    public function addSequenceField($fieldName, &$value, $seqName=null)
    {
      $meta = $this->getDb()->tableMeta($this->m_tables[0]);
      if (!hasFlag($meta[$fieldName]['flags'], MF_AUTO_INCREMENT))
      {
        return parent::addSequenceField($fieldName, $value, $seqName);
      }
      
      $this->m_seqValue = &$value;
      $this->m_seqValue = -1;
      $this->m_seqField = $fieldName;
      $this->m_returnSeqValue = true;
      
      return $this;
    }     
    
    /**
     * Wrapper function to execute an insert query
     */
    public function executeInsert()
    {
      $result = parent::executeInsert();
      
      if ($result && $this->m_returnSeqValue)
      {
        $this->m_seqValue = $this->getDb()->getInsertId();
        atkdebug("Value for sequence column {$this->m_tables[0]}.{$this->m_seqField}: {$this->m_seqValue}");
      }
      
      return $result;
    }    
  }
?>
