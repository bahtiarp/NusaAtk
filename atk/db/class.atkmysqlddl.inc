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
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 6345 $
   * $Id: class.atkmysqlddl.inc 6354 2009-04-15 02:41:21Z mvdam $
   */

  /**
   * MySQL ddl driver.
   *
   * Implements mysql specific ddl statements.
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage db
   *
   */
  class atkMysqlDDL extends atkDDL
  {
    var $m_table_type = NULL;

    /**
     * Constructor
     *
     * @return atkMysqlDDL
     */
    function atkMysqlDDL()
    {
    }
function __construct()
{
  
}
    /**
     * Convert an ATK generic datatype to a database specific type.
     *
     * @param string $generictype  The datatype to convert.
     */
    function getType($generictype)
    {
      $config = atkconfig('db_mysql_default_'.$generictype.'_columntype');
      if ($config) return $config;

      switch ($generictype)
      {
        case "number":
          return "INT";
        case "decimal":
          return "DECIMAL";
        case "string":
          return "VARCHAR";
        case "date":
          return "DATE";
        case "text":
          return "TEXT";
        case "datetime":
          return "DATETIME";
        case "time":
          return "TIME";
        case "boolean":
          return "NUMBER(1,0)"; // size is added fixed. (because a boolean has no size of its own)
      }
      return ""; // in case we have an unsupported type.
    }

    /**
     * Convert an database specific type to an ATK generic datatype.
     *
     * @param string $type  The database specific datatype to convert.
     */
    function getGenericType($type)
    {
      $type = strtolower($type);
      switch($type)
      {
        case "int":
          return "number";
        case "float":
        case "real":
        case "double":
          return "decimal";
        case "varchar":
        case "char":
        case "string":
          return "string";
        case "date":
          return "date";
        case "longtext":
        case "text":
        case "blob":
          return "text";
        case "time":
          return "time";
        case "datetime":
          return "datetime";
      }
      return ""; // in case we have an unsupported type.
    }

    /**
     * Generate a string for a field, to be used inside a CREATE TABLE
     * statement.
     * This function tries to be generic, so it will work in the largest
     * number of databases. Databases that won't work with this syntax,
     * should override this method in the database specific ddl class.
     *
     * @param string $name        The name of the field
     * @param string $generictype The datatype of the field (should be one of the
     *                            generic types supported by ATK).
     * @param int $size        The size of the field (if appropriate)
     * @param int $flags       The DDL_ flags for this field.
     * @param mixed $default   The default value to be used when inserting new
     *                         rows.
     */
    function buildField($name, $generictype, $size=0, $flags=0, $default=NULL)
    {
      if ($generictype == "string" && $size > 255)
        $generictype = "text";

      $result = parent::buildField($name, $generictype, $size, $flags, $default);

      // add binary option after varchar declaration to make sure field
      // values are compared in case-sensitive fashion
      if ($generictype == "string")
        $result = preg_replace('/VARCHAR\(([0-9]+)\)/i', 'VARCHAR(\1) BINARY', $result);

      return $result;
    }

    /**
     * Set all table data at once using the given table meta data,
     * retrieved using the metadata function of the db instance.
     *
     * @param array $tablemeta table meta data array
     */
    function loadMetaData($tablemeta)
    {
    	parent::loadMetaData($tablemeta);
      $this->setTableType($tablemeta[0]["table_type"]);
    }

    /**
     * Sets the table type (for databases that support different
     * table types).
     *
     * @param string $type
     */
    function setTableType($type)
    {
    	$this->m_table_type = $type;
    }

    /**
     * Build a CREATE TABLE query and return it as a string.
     *
     * @return The CREATE TABLE query.
     */
    function buildCreate()
    {
      $query = parent::buildCreate();
      
      if (!empty($this->m_db->m_charset)) {
            $query.= ' DEFAULT CHARACTER SET '.$this->m_db->m_charset;
            if (!empty($this->m_db->m_collate)) {
                $query.= ' COLLATE '.$this->m_db->m_collate;
            }
        }
      
      if (!empty($query) && !empty($this->m_table_type))
      {
        $query .= " TYPE=" .$this->m_table_type;
      }
      return $query;
    }
    
    /**
     * Drop sequence
     *
     * @param string $name Sequence name
     * @return boolean
     */
    function dropSequence($name)
    {
      $table = $this->m_db->quoteIdentifier($this->db->m_seq_table);
      return $this->m_db->query("DELETE FROM $table WHERE ".$this->m_db->quoteIdentifier($this->m_db->m_seq_namefield)." = '".$this->escapeSQL($name)."'");
    }

    /**
     * Rename sequence
     *
     * @param string $name Sequence name
     * @param string $new_name New sequence name
     * @return boolean
     */
    function renameSequence($name,$new_name)
    {
      $name = $this->m_db->escapeSQL($name);
      $new_name = $this->m_db->escapeSQL($new_name);
      return $this->m_db->query("UPDATE db_sequence SET seq_name='$new_name'
                WHERE seq_name='$name'");
    }

    /**
     * Rename table name
     *
     * @param string $name Table name
     * @param string $new_name New table name
     * @return boolean
     */
    function renameTable($name,$new_name)
    {
      $name = $this->m_db->quoteIdentifier($name);
      $new_name = $this->m_db->quoteIdentifier($new_name);
      return $this->m_db->query("ALTER TABLE $name RENAME $new_name");
    }
    
  }
?>
