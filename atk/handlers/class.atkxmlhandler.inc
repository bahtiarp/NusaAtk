<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage handlers
   *
   * @copyright (c)2000-2004 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 3582 $
   * $Id: class.atkxmlhandler.inc 6354 2009-04-15 02:41:21Z mvdam $
   */

  /**
   * Handler class for the exporting a record to an XML file.
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage handlers
   *
   */
  class atkXmlHandler extends atkActionHandler
  {
    /**
     * The action handler method. Creates an xml document and outputs it to the browser.
     */
    function action_xml()
    {
      $recordset = $this->m_node->selectDb($this->m_postvars['atkselector'],"","","","","xml");

      $output = atkOutput::getInstance();

      $document = '<?xml version="1.0"?>'."\n";

      for ($i = 0, $_i = count((array)$recordset); $i < $_i; $i++)
      {
        $document.=$this->invoke("xml", $recordset[$i])."\n";
      }
      $output->output($document);
    }

    /**
     * Convert a record to an XML fragment.
     * @param array $record The record to convert to xml.
     * @return String XML document.
     * @todo This handler can only handle 'simple' key/value attributes
     *       like atkAttribute. Relation support should be added.
     *
     */
    function xml($record)
    {
      $node = $this->m_node;
      $xml = "<".$node->m_type." ";

      $attrs = array();
      foreach (array_keys($node->m_attribList) as $attribname)
      {
        $p_attrib = &$node->m_attribList[$attribname];
        if (!$p_attrib->isEmpty($record))
        {
          $attrs[]=$attribname.'="'.$p_attrib->display($record, "xml").'"';
        }
      }
      if (count((array)$attrs))
      {
        $xml.=implode(" ", $attrs);
      }

      $xml.='/>';

      if (isset($node->m_postvars['tohtml']) && $node->m_postvars['tohtml']==1)
      {
        return htmlspecialchars($xml).'<br>';
      }
      else
      {
        return $xml;
      }
    }
  }

?>