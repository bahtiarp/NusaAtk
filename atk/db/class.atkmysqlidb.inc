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
  *
   */

  /**
   * @internal Include baseclass
   */
  require_once(atkconfig("atkroot")."atk/db/class.atkmysqldb.inc");

  /**
   * Driver for MySQL databases > 4.1.3
   *
   * @author Eldad Ran <eldad@tele-concept.com>
   * @package atk
   * @subpackage db
   */
  class atkMysqliDb extends atkMysqlDb
  {
    /**
     * The last insert id from the last query
     * @var integer
     * @access protected
     */
    var $m_insert_id;
      
    /**
     * Base constructor
     */
    function __construct()
    {
      $this->atkmysqlidb();
    }
    
    function atkmysqlidb()
    {
      if (!function_exists('mysqli_connect'))
      {
        trigger_error('MySQLi not supported by your PHP version',E_USER_ERROR);
      }

      // set type
      $this->m_type = "mysqli";
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
       if (empty($port)) $port = NULL;
       $this->m_link_id = @mysqli_connect($host, $user, $password, $database, $port);
       if (!$this->m_link_id)
       {
         $this->halt($this->getErrorMsg());
         return $this->_translateError();
       }

       /* set character set */
       if (!empty($charset))
       {
         atkdebug("Set database character set to: {$charset}");
         $this->_query("SET NAMES '{$charset}'", true);
       }

       /* set autoCommit to off */
       mysqli_autocommit($this->m_link_id, FALSE);
     }


     /* return link identifier */
     return DB_SUCCESS;
   }

   /**
    * Store MySQL errors in internal variables
    * @access private
    */
   function _setErrorVariables()
   {
     if(!empty($this->m_link_id))
     {
       $this->m_errno = mysqli_errno($this->m_link_id);
       $this->m_error = mysqli_error($this->m_link_id);
     }
     else
     {
       $this->m_errno = mysqli_connect_errno();
       $this->m_error = mysqli_connect_error();
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
       @mysqli_close($this->m_link_id);
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
       return @mysqli_real_escape_string($this->m_link_id, $string);
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

     /* connect to database */
     $mode = $this->getQueryMode($query);
     if ($this->connect($mode)==DB_SUCCESS)
     {
       /* free old results */
       if ($this->m_query_id)
       {
         if (is_resource($this->m_query_id))
          mysqli_free_result($this->m_query_id);
         $this->m_query_id = 0;
       }

       $this->m_affected_rows = 0;

       /* query database */
       $this->m_query_id = $this->_query($query, false);
      
		if($mode == 'r')
        {   
            $this->m_num_rows = @mysqli_num_rows($this->m_query_id);
        }	 
       /* get the last insert id
        * this is harmless and returns 0 if the query wasn't
        * an insert or update or if the table has no autoincrement
        */
       $this->m_insert_id = mysqli_insert_id($this->m_link_id);

       $unlock_table = false;
       if (mysqli_errno($this->m_link_id) == 1100)
       {
         $this->locktables_fallback_on_error($query,$mode);
         $unlock_table = true;
       }
       else
       {
         $this->m_affected_rows = mysqli_affected_rows($this->m_link_id);
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

       if (atkconfig('debug') >= 1)
       {
         $this->debugWarnings();
       }

       return true;
     }
     return false;
   }
   
   /**
    * Execute and log query.
    * 
    * @param string  $query          query
    * @param boolean $isSystemQuery  is system query? (e.g. for retrieving metadata, warnings, setting locks etc.)
    */
   protected function _query($query, $isSystemQuery)
   {
     if (atkconfig("debug") >= 0)
     {
       atkimport("atk.utils.atkdebugger");
       atkDebugger::addQuery($query, $isSystemQuery);
     }
          
     return @mysqli_query($this->m_link_id, $query);
   }

   /**
    * Get all MySQL warnings for the previously executed query
    * and make atkwarnings of them.
    */
   public function debugWarnings()
   {
     $stmt = $this->_query('SHOW WARNINGS', true);

     $warnings = array();
     while ($warning = $stmt->fetch_assoc())
     {
       $warnings[] = $warning;
     }

     foreach ($warnings as $warning)
     {
       atkwarning("MYSQL warning '{$warning['Level']}' (Code: {$warning['Code']}): {$warning['Message']}");
     }
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
     $error = mysqli_error($this->m_link_id);

     $matches = array();
     preg_match("/\'(.*)\'/U",$error,$matches);

     if (is_array($matches) && sizeof($matches) == 2)
     {
       atkdebug("<b>Fallback feature called because error '1100' occured during the last query. Running query again using table lock for table '{$matches[1]}'.</b>");
       $table = $matches[1];

       if ($this->m_query_id)
       {
         if (is_resource($this->m_query_id))
          mysqli_free_result($this->m_query_id);
         $this->m_query_id = 0;
       }
       $this->m_affected_rows = 0;

       $this->lock($table,($querymode == 'r' ? 'read' : 'write'));
       $this->m_query_id = $this->_query($query, true);

       $this->m_affected_rows = mysqli_affected_rows($this->m_link_id);
     }
   }

   /**
    * Goto the next record in the result set
    * @return result of going to the next record
    */
   function next_record()
   {
     /* goto next record */
     $this->m_record = @mysqli_fetch_array($this->m_query_id, MYSQLI_ASSOC|atkconfig("mysqlfetchmode"));
     $this->m_row++;
     $this->m_errno = mysqli_errno($this->m_link_id);
     $this->m_error = mysqli_error($this->m_link_id);

     /* are we there? */
     $result = is_array($this->m_record);
     if (!$result && $this->m_auto_free)
     {
       @mysqli_free_result($this->m_query_id);
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
     $result = @mysqli_data_seek($this->m_query_id, $position);
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
     if ($this->connect('w')==DB_SUCCESS)
     {
       /* lock */
       $query = "LOCK TABLES $table $mode";

       if (atkconfig("debug")>=0)
       {
         atkimport("atk.utils.atkdebugger");
         atkDebugger::addQuery($query);
       }

       $result = $this->_query($query, true);
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
     if ($this->connect('w')==DB_SUCCESS)
     {
       /* unlock */
       atkdebug("unlock tables");
       $result = $this->_query("UNLOCK TABLES", true);
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
     return $this->m_affected_rows;
   }

   /**
    * Evaluate the result; how many rows
    * were affected by the query.
    * @return number of affected rows
    */
   function num_rows()
   {
     //return @mysqli_num_rows($this->m_query_id);
		return $this->m_num_rows;
   }

   /**
    * Evaluatie the result; how many fields
    * where affected by the query.
    * @return number of affected fields
    */
   function num_fields()
   {
     return @mysqli_num_fields($this->m_query_idD);
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
     if ($this->connect('w')==DB_SUCCESS)
     {
       /* lock sequence table */
       if ($this->lock($this->m_seq_table))
       {
         /* get sequence number (locked) and increment */
         $query = "SELECT ".$this->m_seq_field." FROM ".$this->m_seq_table." WHERE ".$this->m_seq_namefield." = '$sequence'";

         $id = $this->_query($query, true);
         $result = @mysqli_fetch_array($id);

         /* no current value, make one */
         if (!is_array($result))
         {
           $query = "INSERT INTO ".$this->m_seq_table." VALUES('$sequence', 1)";
           $id = $this->_query($query, true);
           $this->unlock();
           return 1;
         }

         /* enter next value */
         else
         {
           $nextid = $result[$this->m_seq_field] + 1;
           $query = "UPDATE ".$this->m_seq_table." SET ".$this->m_seq_field." = '$nextid' WHERE ".$this->m_seq_namefield." = '$sequence'";

           $id = $this->_query($query, true);
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
    * This function checks the database for a table with
    * the provide name
    *
    * @param String $table the table to find
    * @return boolean true if found, false if not found
    */
   function tableExists($table)
   {
     $this->connect('r');
     if (strpos($table, '.') !== false)
     {
       list($dbname, $tablename) = explode(".", $table);
       $id = $this->_query("SHOW TABLES FROM `". $dbname ."` LIKE '". $tablename ."'", true);
     }
     else
     {
       $id = $this->_query("SHOW TABLES LIKE '" .$table ."'", true);
     }
     
     $result = @mysqli_num_rows($id) > 0;
     atkdebug("Table exists? $table => ".($result ? 'yes' : 'no'));
     return $result;
   }   

   /**
    * Returns the table type.
    *
    * @param string $table table name
    * @return string table type
    */
   function _getTableType($table)
   {
     $this->connect('r');
     $id = $this->_query("SHOW TABLE STATUS LIKE '" .$table ."'", true);
     $status = @mysqli_fetch_array($id, MYSQLI_ASSOC|atkconfig("mysqlfetchmode"));
     $result = $status != NULL && isset($status['Engine']) ? $status['Engine'] : NULL;
     atkdebug("Table type? $table => $result");
     return $result;
   }   
   
   /**
    * Return the meta data of a certain table
    * @param string $table the table name
    * @param bool $full all meta data or not
    * @return array with meta data
    */
   function metadata($table, $full=false)
   {

     /* first connect */
     if ($this->connect('r')==DB_SUCCESS)
     {
       atkimport("atk.db.atkddl");
       $ddl = atkDDL::create("mysqli");

       /* list fields */
       atkdebug("Retrieving metadata for $table");
     
       /* The tablename may also contain a schema. If so we check for it. */
       if (strpos($table, ".") !== false)
       {
         list($dbname, $tablename) = explode(".", $table);
         
         /* get meta data */
         $id = @$this->_query("SELECT * FROM `{$dbname}`.`{$tablename}` LIMIT 1", true);
       }
       else
       {
         /* get meta data */
         $id = $this->_query("SELECT * FROM `{$table}` LIMIT 1", true);
       }

       // table type
       $tableType = $this->_getTableType(isset($tablename)? $tablename : $table);
       
       if (!$id)
       {
         	atkdebug("Metadata query failed.");
         	return array();
       }
       $i  = 0;
       $result = array();

       while ($finfo = mysqli_fetch_field($id))
       {
		      $result[$i]["table"]      = $finfo->table;
		      $result[$i]["table_type"] = $tableType;
	        $result[$i]["name"]       = $finfo->name;
	        $result[$i]["type"]       = $finfo->type;
	        $result[$i]["gentype"]    = $ddl->getGenericType($finfo->type);
	        $result[$i]["len"]        = $finfo->length;
	        $result[$i]["flags"]      = 0;

	        if ($result[$i]["gentype"] == "decimal")
	        {
	          $result[$i]["len"] -= $finfo->decimals + 1;
	          $result[$i]["len"] .= "," . $finfo->decimals;
	        }

	        if($finfo->flags & MYSQLI_PRI_KEY_FLAG) $result[$i]["flags"]|= MF_PRIMARY;
	        if($finfo->flags & MYSQLI_UNIQUE_KEY_FLAG) $result[$i]["flags"]|= MF_UNIQUE;
	        if($finfo->flags & MYSQLI_NOT_NULL_FLAG ) $result[$i]["flags"]|= MF_NOT_NULL;
	        if($finfo->flags & MYSQLI_AUTO_INCREMENT_FLAG) $result[$i]["flags"]|= MF_AUTO_INCREMENT;

         	if ($full)
           		$result["meta"][$result[$i]["name"]] = $i;
	      	$i++;
      	}

      	if ($full)
          $result["num_fields"] = $i;

        mysqli_free_result($id);

        atkdebug("Metadata for $table complete");
        return $result;
     }
     return array();
   }

   /**
    * Return the available table names
    *
    * @param bool $includeViews Include views?
    * @return array with table names etc.
    */
   function table_names($includeViews=true)
   {
     // query
     $this->query("SHOW ".(!$includeViews ? "FULL" : "")." TABLES");

     // get table names
     $result = array();
     for ($i=0; $info = mysqli_fetch_row($this->m_query_id); $i++)
     {
       // ignore views?
       if (!$includeViews && strtoupper($info[1]) == 'VIEW') continue;

       $result[$i]["table_name"]      = $info[0];
       $result[$i]["tablespace_name"] = $this->m_database;
       $result[$i]["database"]        = $this->m_database;
     }

     // return result
     return $result;
   }

   /**
    * Commit the current transaction.
    *
    * @return bool true
    */
   function commit()
   {
     if($this->m_link_id)
     {
       atkdebug("Commit");
       mysqli_commit($this->m_link_id);
     }
     return true;
   }

   /**
    * Set savepoint with the given name.
    *
    * @param string $name savepoint name
    */
   function savepoint($name)
   {
     atkdebug(get_class($this)."::savepoint $name");
     $this->query('SAVEPOINT '.$name);
   }

   /**
    * Rollback the the current transaction.
    *
    * @param string $savepoint The savepoint to rollback to
    */
   function rollback($savepoint="")
   {
     if ($this->m_link_id)
     {
       if (!empty($savepoint))
       {
         atkdebug(get_class($this)."::rollback (rollback to savepoint $savepoint)");
         $this->query('ROLLBACK TO SAVEPOINT '.$savepoint);
       }
       else
       {
         atkdebug("Rollback");
         mysqli_rollback($this->m_link_id);
       }
     }

     return true;
   }

   /**
    * Enable/disable all foreign key constraints.
    *
    * @param boolean $enable enable/disable foreign keys?
    */
   function toggleForeignKeys($enable)
   {
     $this->query("SET FOREIGN_KEY_CHECKS = ".($enable ? 1 : 0));
   }

   /**
    * Returns the last inserted auto increment value.
    *
    * @return int auto increment value of latest insert query
    */
   public function getInsertId()
   {
     return $this->m_insert_id;
   }
 }
?>
