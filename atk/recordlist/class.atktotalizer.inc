<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage recordlist
   *
   * @copyright (c)2003-2006 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 6320 $
   * $Id: class.atktotalizer.inc 6354 2009-04-15 02:41:21Z mvdam $
   */

  /**
   * The atkTotalizer is a utility class to handle totals and subtotals
   * in recordlists.
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage recordlist
   */
  class atkTotalizer
  {
    var $m_node = NULL;
    var $m_columnConfig = NULL;

    /**
     * Constructor
     *
     * @param atkNode $node
     * @param atkColumnConfig $columnConfig
     * @return atkTotalizer
     */
    function atkTotalizer(&$node, &$columnConfig)
    {
      $this->m_node = &$node;
      $this->m_columnConfig = &$columnConfig;
    }

    /**
     * Totalize the recordset
     *
     * @param array $rowset
     * @return array
     */
    function totalize($rowset)
    {
      $result = array();
      $lastvalues = array();

      $totalizers = $this->m_columnConfig->totalizableColumns();
      $subtotalfields = $this->m_columnConfig->subtotalColumns();

      $totals = array();

      for($i=0, $_i=count((array)$rowset); $i<$_i; $i++)
      {
        $record = $rowset[$i]["record"];
        for ($j=count((array)$subtotalfields)-1; $j>=0; $j--) // reverse loop, to ensure right-to-left subtotalling
        {
          $fieldname = $subtotalfields[$j];
          $value = $record[$fieldname];
          $p_subtotalling_attrib = $this->m_node->m_attribList[$fieldname];

          if (isset($lastvalues[$fieldname]) && !$p_subtotalling_attrib->equal($record, $lastvalues))
          {
            $result[] = $this->_subTotalRow($rowset[$i], $totals, $fieldname, $totalizers);
          }

          foreach($totalizers as $totalfield)
          {
            $p_attrib = $this->m_node->getAttribute($totalfield);
            $totals[$totalfield][$fieldname] = $p_attrib->sum($totals[$totalfield][$fieldname], $record);
          }
          $lastvalues[$fieldname] = $value;
        }

        $result[] = $rowset[$i];

      }
      // leftovers, subtotals of last rows
      if (count((array)$rowset))
      {
        for ($j=count((array)$subtotalfields)-1; $j>=0; $j--) // reverse loop, to ensure right-to-left subtotalling
        {
          $fieldname = $subtotalfields[$j];
          $result[] = $this->_subTotalRow($rowset[count((array)$rowset)-1], $totals, $fieldname, $totalizers);
        }
      }
      // end of leftovers

      return $result;
    }

    /**
     * Totalize one row
     *
     * @param array $row
     * @param array $totals
     * @param string $fieldforsubtotal
     * @param array $totalizers
     * @return array
     */
    function _subTotalRow($row, &$totals, $fieldforsubtotal, $totalizers)
    {
      $subtotalcols = array();
      foreach($totalizers as $totalfield)
      {
        $p_attrib = $this->m_node->m_attribList[$totalfield];
        $subtotalcols[$totalfield] = $p_attrib->display($totals[$totalfield][$fieldforsubtotal]);

        // reset walking total
        $totals[$totalfield][$fieldforsubtotal] = "";
      }


      return $this->_createSubTotalRowFromRow($row, $fieldforsubtotal, $subtotalcols);
    }

    /**
     * Create subtotal row from row
     *
     * @param array $row
     * @param string $fieldname
     * @param array $subtotalcolumns
     * @return array
     */
    function _createSubTotalRowFromRow($row, $fieldname, $subtotalcolumns)
    {
      // fix type
      $row["type"] = "subtotal";

      // replace columns
      foreach($row["data"] as $col=>$value)
      {
        if ($col==$fieldname)
        {
          $row["data"][$col] = atktext("subtotal");
        }
        else if (isset($subtotalcolumns[$col]))
        {
          $row["data"][$col] = $subtotalcolumns[$col];
        }
        else
        {
          $row["data"][$col]="";
        }
      }
      return $row;
    }
  }

?>