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
   * @version $Revision: 6309 $
   * $Id: class.atkcreatestampattribute.inc 6354 2009-04-15 02:41:21Z mvdam $
   */

  /**
   * @internal include baseclass.
   */
  useattrib('atkUpdateStampAttribute');
  
  /**
   * Attribute for keeping track of record creation times.
   *
   * The atkCreateStampAttribute class can be used to automatically store the
   * date and time of the creation of a record.
   * To use this attribute, add a DATETIME field to your table and add this
   * attribute to your node. No params are necessary, no initial_values need 
   * to be set. The timestamps are generated automatically.
   * This attribute is automatically set to readonly, and to af_hide_add 
   * (because we only have the timestamp AFTER a record is added).
   *
   * (the attribute was posted at www.achievo.org/forum/viewtopic.php?p=8608)
   *
   * @author Rich Kucera <kucerar@hhmi.org>
   * @package atk
   * @subpackage attributes
   *
   */
  class atkCreateStampAttribute extends atkUpdateStampAttribute 
  {

    /**
     * Constructor
     *
     * @param String $name Name of the attribute (unique within a node, and
     *                     corresponds to the name of the datetime field
     *                     in the database where the stamp is stored.
     * @param int $flags Flags for the attribute.
     */
    function atkCreateStampAttribute($name, $flags=0)
    {
      $this->atkUpdateStampAttribute($name, $flags);
    }

    function __construct($name, $flags=0)
    {
      $this->atkUpdateStampAttribute($name, $flags);
    }
    /**
     * This function is called by the framework to determine if the attribute
     * needs to be saved to the database in an updateDb call.
     * This attribute should never be updated
     *
     * @param array $rec The record that is going to be saved.
     * @return boolean True if this attribute should participate in the update
     *                 query; false if not.
     */
    function needsUpdate($rec)
    {
      // no matter what, we NEVER save a new value.
      return false;
    }

  } 

?>
