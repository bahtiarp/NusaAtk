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
   * @version $Revision: 6344 $
   * $Id: class.atkmysqldb.inc 6759 2010-03-09 12:47:30Z patrick $
   */

  /**
   * Driver for MySQL databases.
   *
   * @author Peter Verhage <peter@ibuildings.nl>
   * @package atk
   * @subpackage db
   */
  class atkMysqlDb extends atkDb
  {

    /* sequence table */
    var $m_seq_table = "db_sequence";
    // the field in the seq_table that contains the counter..
    var $m_seq_field = "nextid";
    // the field in the seq_table that countains the name of the sequence..
    var $m_seq_namefield = "seq_name";

    var $m_type = "mysql";

    protected $m_identifierQuoting = array('start' => '`', 'end' => '`', 'escape' => '`');


    /**
     * Base constructor
     */
    function atkmysqldb()
    {
      if (!function_exists('mysql_connect'))
      {
        atkerror('MySQL not supported by your PHP version');
        die('MySQL not supported by your PHP version');
      }

      // set the user error's
      $this->m_type = "mysql";
      $this->m_vendor = "mysql";
      $this->m_user_error=array(1451);
    }

   function __construct()
    {
      if (!function_exists('mysql_connect'))
      {
        atkerror('MySQL not supported by your PHP version');
        die('MySQL not supported by your PHP version');
      }

      // set the user error's
      $this->m_type = "mysql";
      $this->m_vendor = "mysql";
      $this->m_user_error=array(1451);
    }

    /**
     * Connect to the database
     *
     * @param string $host Hostname
     * @param string $user Username
     * @param string $password Password
     * @param string $database The database to connect to
     * @param int $port The portnumber to use for connecting
     * @param string $charset The charset to use
     * @return mixed Connection status
     */
    function doConnect($host,$user,$password,$database,$port,$charset)
    {
      /* establish connection */
      if (empty($this->m_link_id))
      {
        if (!empty($port))
          $host = "$host:$port";


        $this->m_link_id = @mysql_connect($host, $user, $password, true /* ALWAYS NEW LINK */);
        if (!$this->m_link_id)
        {
          $this->halt($this->getErrorMsg());
        }

        /* set character set */
        if (!empty($charset))
        {
          atkdebug("Set database character set to: {$charset}");
          @mysql_query("SET NAMES '{$charset}'", $this->m_link_id);
        }
      }

      /* select database */
      if (!@mysql_select_db($this->m_database, $this->m_link_id))
      {
        $this->halt($this->getErrorMsg());
        return $this->_translateError();
      }

      /* return link identifier */
      return DB_SUCCESS;
    }

    /**
     * Determine whether an error that occurred is a recoverable (user) error
     * or a system error.
     * @return String "user" or "system"
     */
    function getErrorType()
    {
      $this->_setErrorVariables();
      return parent::getErrorType();
    }

    /**
     * Translates known database errors to developer-friendly messages
     *
     * @return int Flag of the error
     */
    function _translateError($errno = 0)
    {
      $this->_setErrorVariables();
      switch ($this->m_errno)
      {
        case    0: return DB_SUCCESS;
        case 1044: return DB_ACCESSDENIED_DB;  // todofixme: deze komt bij mysql pas na de eerste query.
        case 1045: return DB_ACCESSDENIED_USER;
        case 1049: return DB_UNKNOWNDATABASE;
        case 2004:
        case 2005: return DB_UNKNOWNHOST;
        default:
          atkdebug("mysqldb::translateError -> MySQL Error: ".
                    $this->m_errno." -> ".$this->m_error);
          return DB_UNKNOWNERROR;
      }
    }

    /**
     * Store MySQL errors in internal variables
     * @access private
     */
    function _setErrorVariables()
    {
      if (!empty($this->m_link_id))
      {
        $this->m_errno = mysql_errno($this->m_link_id);
        $this->m_error = mysql_error($this->m_link_id);
      }
      else
      {
        $this->m_errno = mysql_errno();
        $this->m_error = mysql_error();
      }
    }

    /**
     * Disconnect from database
     */
    function disconnect()
    {
      if ($this->m_link_id)
      {
        atkdebug("Disconnecting from database...");
        @mysql_close($this->m_link_id);
        $this->m_link_id = 0;
      }
    }

   /**
    * Escaping a MySQL string, in a mysqli safe way
    * @param string $string
    * @param bool $wildcard
    */
   function escapeSQL($string, $wildcard=false)
   {
      if ($this->connect('r') === DB_SUCCESS)
      {
        if ($wildcard == true)
        {
          $string = str_replace('%', '\%', $string);
        }
        return mysql_real_escape_string($string, $this->m_link_id);
      }
      return null;
    }

    /**
     * Performs a query
     * @param string $query the query
     * @param int $offset offset in record list
     * @param int $limit maximum number of records
     */
    function query($query, $offset=-1, $limit=-1)
    {
      /* limit? */
      if ($offset >= 0 && $limit > 0)
        $query .= " LIMIT $offset, $limit";

      if (atkconfig("debug")>=0)
      {
        atkimport("atk.utils.atkdebugger");
        atkDebugger::addQuery($query);
      }

      $mode = $this->getQueryMode($query);

      /* connect to database */
      if ($this->connect($mode)==DB_SUCCESS)
      {
        /* free old results */
        if ($this->m_query_id)
        {
          if (is_resource($this->m_query_id))
            mysql_free_result($this->m_query_id);
          $this->m_query_id = 0;
        }

        $this->m_affected_rows = 0;

        /* query database */
        $this->m_query_id = @mysql_query($query, $this->m_link_id);

        $unlock_table = false;
        if (mysql_errno($this->m_link_id) == 1100)
        {
          $this->locktables_fallback_on_error($query, $mode);
          $unlock_table = true;
        }
        else
        {
          $this->m_affected_rows = mysql_affected_rows();
        }

        $this->m_row = 0;

        /* invalid query */
        if (!$this->m_query_id)
        {
          $this->halt("Invalid SQL: $query");
          return false;
        }

        if ($unlock_table)
          $this->unlock();

        /* return query id */
        return true;
      }
      return false;
    }

    /**
     * This method provides a fallback when error 1100 occurs
     * (Table ... not locked using LOCK TABLES). This method locks
     * the table and runs the query again.
     *
     * @param string $query The original query that failed
     * @param string $querymode Kind of query - 'w' for write or 'r' for read
     */
    function locktables_fallback_on_error($query, $querymode='w')
    {
      $error = mysql_error($this->m_link_id);
      $matches = array();

      preg_match("/\'(.*)\'/U",$error,$matches);

      if (is_array($matches) && sizeof($matches) == 2)
      {
        atkdebug("<b>Fallback feature called because error '1100' occured during the last query. Running query again using table lock for table '{$matches[1]}' (".$this->m_link_id.").</b>");
        $table = $matches[1];

        if ($this->m_query_id)
        {
          if (is_resource($this->m_query_id))
            mysql_free_result($this->m_query_id);
          $this->m_query_id = 0;
        }
        $this->m_affected_rows = 0;

        $this->lock($table,($querymode=='r' ? 'read' : 'write'));
        $this->m_query_id = @mysql_query($query, $this->m_link_id);

        $this->m_affected_rows = mysql_affected_rows($this->m_link_id);
      }
    }

    /**
     * Goto the next record in the result set
     * @return result of going to the next record
     */
    function next_record()
    {
      /* goto next record */
      $this->m_record = @mysql_fetch_array($this->m_query_id, MYSQL_ASSOC|atkconfig("mysqlfetchmode"));
      $this->m_row++;
      $this->m_errno = mysql_errno();
      $this->m_error = mysql_error();

      /* are we there? */
      $result = is_array($this->m_record);
      if (!$result && $this->m_auto_free)
      {
        @mysql_free_result($this->m_query_id);
        $this->m_query_id = 0;
      }

      /* return result */
      return $result;
    }

    /**
     * Goto a certain position in result set.
     * Not specifying a position will set the pointer
     * at the beginning of the result set.
     * @param int $position the position
     */
    function seek($position=0)
    {
      $result = @mysql_data_seek($this->m_query_id, $position);
      if ($result) $this->m_row = $position;
      else $this->halt("seek($position) failed: result has ".$this->num_rows()." rows");
    }

    /**
     * Lock a certain table in the database
     * @param string $table the table name
     * @param string $mode the type of locking
     * @return result of locking
     */
    function lock($table, $mode="write")
    {
      /* connect first */
      if ($this->connect("w")==DB_SUCCESS)
      {
        $query = "lock tables $table $mode";

        if (atkconfig("debug")>=0)
        {
          atkimport("atk.utils.atkdebugger");
          atkDebugger::addQuery($query);
        }

        /* lock */
        $result = @mysql_query($query, $this->m_link_id);
        if (!$result) $this->halt("$mode lock on $table failed.");

        /* return result */
        return $result;
      }
      return 0;
    }

    /**
     * Unlock table(s) in the database
     * @return result of unlocking
     */
    function unlock()
    {
      /* connect first */
      if ($this->connect("w")==DB_SUCCESS)
      {
        /* unlock */
        $result = @mysql_query("unlock tables",$this->m_link_id);
        if (!$result) $this->halt("unlock tables failed.");

        /* return result */
        return $result;
      }
      return 0;
    }

    /**
     * Evaluate the result; which rows were
     * affected by the query.
     * @return affected rows
     */
    function affected_rows()
    {
      return @mysql_affected_rows($this->m_link_id);
    }

    /**
     * Evaluate the result; how many rows
     * were affected by the query.
     * @return number of affected rows
     */
    function num_rows()
    {
      return @mysql_num_rows($this->m_query_id);
    }

    /**
     * Evaluatie the result; how many fields
     * where affected by the query.
     * @return number of affected fields
     */
    function num_fields()
    {
      return @mysql_num_fields($this->m_query_idD);
    }

    /**
     * Get the next sequence number
     * of a certain sequence.
     * @param string $sequence the sequence name
     * @return the next sequence id
     */
    function nextid($sequence)
    {
      /* first connect */
      if ($this->connect("w")==DB_SUCCESS)
      {
        /* lock sequence table */
        if ($this->lock($this->m_seq_table))
        {
          /* get sequence number (locked) and increment */
          $query = "SELECT ".$this->m_seq_field." FROM ".$this->m_seq_table." WHERE ".$this->m_seq_namefield." = '$sequence'";

          $id = @mysql_query($query, $this->m_link_id);
          $result = @mysql_fetch_array($id);

          /* no current value, make one */
          if (!is_array($result))
          {
            $query = "INSERT INTO ".$this->m_seq_table." VALUES('$sequence', 1)";
            $id = @mysql_query($query, $this->m_link_id);
            $this->unlock();
            return 1;
          }

          /* enter next value */
          else
          {
            $nextid = $result[$this->m_seq_field] + 1;
            $query = "UPDATE ".$this->m_seq_table." SET ".$this->m_seq_field." = '$nextid' WHERE ".$this->m_seq_namefield." = '$sequence'";

            $id = @mysql_query($query, $this->m_link_id);
            $this->unlock();
            return $nextid;
          }
        }
        return 0;
      }

      /* cannot connect */
      else
      {
        $this->halt("cannot connect to ".$this->m_host);
      }
    }


      /**
     * Drop all database tables.
     */
    function dropAll()
    {
      $tables = $this->table_names();
      foreach($tables as $table)
      {

        $this->query("DROP TABLE `".$table['table_name']."`");
      }
    }

    /**
     * Returns the table type.
     *
     * @param string $table table name
     * @return string table type
     */
    function _getTableType($table)
    {
      list($status) = $this->getRows("SHOW TABLE STATUS LIKE '" .$table ."';");
      return $status != NULL && isset($status['Engine']) ? $status['Engine'] : NULL;
    }

    /**
     * Return the meta data of a certain table
     * @param String $table the table name (optionally in 'database.tablename' format)
     * @param boolean $full all meta data or not
     * @return array with meta data
     */
    function metadata($table, $full=false)
    {
      /* first connect */
      if ($this->connect("r")==DB_SUCCESS)
      {
        atkimport("atk.db.atkddl");
        $ddl = atkDDL::create("mysql");

        /* list fields */
        atkdebug("Retrieving metadata for $table");

        // table type
        $tableType = $this->_getTableType($table);

        // Yury: $table name may contain db name like "dbname.tablename", so we check it
        if (strpos($table, ".")!==false)
        {
          list($dbname,$tablename) = explode(".",$table);
          $id = @mysql_list_fields($dbname, $tablename);
        }
        else
        {
          $id = @mysql_list_fields($this->m_database, $table);
        }

        if (!$id)
        {
          atkdebug("Metadata query failed.");
          return array();
        }

        /* count fields */
        $count = @mysql_num_fields($id);

        /* get meta data */
        $result = array();

        if ($full)
        {
          $result["num_fields"] = $count;
        }

        for ($i=0; $i<$count; $i++)
        {
          $result[$i]["table"]      = @mysql_field_table($id, $i);
          $result[$i]["table_type"] = $tableType;
          $result[$i]["name"]       = @mysql_field_name($id, $i);
          $result[$i]["type"]       = @mysql_field_type($id, $i);
          $result[$i]["gentype"]    = $ddl->getGenericType($result[$i]['type']);

          $len = $this->getColumnLength($table, $result[$i]["name"], $result[$i]["gentype"]);
          if ($len === false)
          {
            $len = @mysql_field_len($id, $i);
          }

          $result[$i]["len"]        = $len;

          $result[$i]["flags"]      = explode(' ', @mysql_field_flags($id, $i));
          $result[$i]["flags"]      =
            (in_array('primary_key',    $result[$i]["flags"]) ? MF_PRIMARY        : 0) |
            (in_array('unique_key',     $result[$i]["flags"]) ? MF_UNIQUE         : 0) |
            (in_array('not_null',       $result[$i]["flags"]) ? MF_NOT_NULL       : 0) |
            (in_array('auto_increment', $result[$i]["flags"]) ? MF_AUTO_INCREMENT : 0);

          if ($full)
          {
            $result["meta"][$result[$i]["name"]] = $i;
          }
        }

        /* free result */
        @mysql_free_result($id);
        atkdebug("Metadata for $table complete");
        return $result;
      }
      return array();
    }

    /**
     * Mysql_field_len returns unusable results for decimal,float and double column types.
     * This method returns a number format, e.g.: '6,2' or false in case of failure OR
     * if the field type does not support a floating point (e.g. for integer fields) so it can
     * fall back on mysql_field_len()
     *
     * @param string $tableName
     * @param string $columnName
     * @param string $columnType
     * @return false|string e.g. '6,2'
     */
    public function getColumnLength($tableName, $columnName, $columnType)
    {
      // Lookup cache
      static $metaData  = array();

      // We check for a ATK normalized column type !
      if ($columnType === 'decimal')
      {
        $columnName = strtolower($columnName);

        // Check for 'cached' results, to avoid query lookups
        if (!isset($metaData[ $tableName ][ $columnName ]))
        {
          $metaQuery  = mysql_query('SHOW COLUMNS FROM '. $tableName, $this->m_link_id);
          while (($metaResult = mysql_fetch_assoc($metaQuery)) !== false)
          {
            $metaData[ $tableName ][ strtolower($metaResult['Field']) ] = $metaResult;
          }
        }

        // If this would return false, we query for a non-existant column name
        if (isset($metaData[ $tableName ][ $columnName ]['Type']))
        {
          $type = $metaData[ $tableName ][ $columnName ]['Type'];

          // Figuring out the length output.
          if (($open = strpos($type, '(')) !== false)
          {
            $close  = strpos($type, ')', $open) - 1;
            return substr($type, $open + 1, $close - $open);
          }
          else
          {
            atkwarning('Unexpected column type format, falling back on mysql_field_len()');
          }
        }
      }

      // Returning false, expecting a fall back on mysql_field_len()
      return false;
    }

    /**
     * Return the available table names
     * @return array with table names etc.
     */
    function table_names($includeViews = true)
    {
      /* query */
      $this->query("SHOW TABLES");

      /* get table names */
      $result = array();
      for ($i=0; $info = mysql_fetch_row($this->m_query_id); $i++)
      {
        $result[$i]["table_name"]      = $info[0];
        $result[$i]["tablespace_name"] = $this->m_database;
        $result[$i]["database"]        = $this->m_database;
      }

      /* return result */
      return $result;
    }

    /**
     * This function checks the database for a table with
     * the provide name
     *
     * @param String $table the table to find
     * @return boolean true if found, false if not found
     */
    function tableExists($table)
    {
      $res = $this->getrows("SHOW TABLES LIKE '$table'");
      return (count((array)$res)==0 ? false : true);
    }

    /**
     * This function indicates what searchmodes the database supports.
     * @return array with search modes
     */
    function getSearchModes()
    {
      return array("exact","substring","wildcard","regexp","soundex","greaterthan","greaterthanequal","lessthan","lessthanequal","between");
    }

     /**
     * Get TO_CHAR() equivalent for the current database.
     * Each database driver should override this method to perform vendor
     * specific conversion.
     *
     * @param String $fieldname The field to generate the to_char for.
     * @param String $format Format specifier. The format is compatible with
     *                       php's date() function (http://www.php.net/date)
     *                       The default is what's specified by
     *                       $config_date_to_char, or "Y-m-d" if not
     *                       set in the configuration.
     * @return String Piece of sql query that converts a date field to char
     *                for the current database
     */
    function func_datetochar($fieldname, $format="")
    {
      if ($format=="") $format = atkconfig("date_to_char", "Y-m-d");
      /*Patching by bahtiarp@gmail.com 2 Nop 2015*/
     // return "DATE_FORMAT($fieldname, '".$this->vendorDateFormat($format)."')";
     return $fieldname;
    }

    /**
     * Convert a php date() format specifier to a mysql specific format
     * specifier.
     *
     * Note that currently, only the common specifiers Y, m, d, H, h, i and
     * s are supported.
     * @param String $format Format specifier. The format is compatible with
     *                       php's date() function (http://www.php.net/date)
     * @return String Mysql specific format specifier.
     */
    function vendorDateFormat($format)
    {
      $php_fmt = array("Y", "m", "d", "H", "h", "i", "s");
      $db_fmt  = array("%Y", "%m", "%d", "%H", "%h", "%i", "%s");
      return str_replace($php_fmt, $db_fmt, $format);
    }


    /**
     * Get TO_CHAR() equivalent for the current database.
     *
     * TODO/FIXME: add format parameter. Current format is always yyyy-mm-dd hh:mi.
     *
     * @param String $fieldname The field to generate the to_char for.
     * @return String Piece of sql query that converts a datetime field to char
     *                for the current database
     */
    function func_datetimetochar($fieldname)
    {
      return "DATE_FORMAT($fieldname, '%Y-%m-%d %H:%i')";
    }

    /**
     * Set database sequence value.
     *
     * @param string $seqname sequence name
     * @param int $value sequence value
     */
    function setSequenceValue($seqname, $value)
    {
      $value = $value + 1; // nextid should contain the next value, not the current value

      if ($this->lock($this->m_seq_table))
      {
        $query = "SELECT COUNT(*) AS result FROM ".$this->m_seq_table." WHERE ".$this->m_seq_namefield." = '$seqname'";

        $id = @mysql_query($query, $this->m_link_id);
        $result = @mysql_fetch_array($id);

        if ($result[0]['result'] == 0)
        {
          $query = "INSERT INTO ".$this->m_seq_table." VALUES('$seqname', $value)";
        }
        else
        {
          $query = "UPDATE ".$this->m_seq_table." SET ".$this->m_seq_field." = '$value' WHERE ".$this->m_seq_namefield." = '$seqname'";
        }

        $this->query($query);

        $this->unlock();
      }
    }
  }
?>
