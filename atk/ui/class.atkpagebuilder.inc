<?php
/**
 * This file is part of the Achievo ATK distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package atk
 * @subpackage ui
 *
 * @copyright (c) 2000-2008 Ivo Jansch
 * @license http://www.achievo.org/atk/licensing ATK Open Source License
 *
 * @version $Revision: 5835 $
 * $Id: class.atkpagebuilder.inc 6354 2009-04-15 02:41:21Z mvdam $
 */

/**
 * Page builder. Provides a fluent interface to create standardized ATK pages.
 * 
 * $node->createPageBuilder()
 *      ->title('...')
 *      ->beginActionBox()
 *        ->formStart('...')
 *        ->content('...')
 *      ->endActionBox()
 *      ->box('...')
 *      ->render();
 *
 * @author Peter C. Verhage <peter@achievo.org>
 * @package atk
 * @subpackage ui
 */
class atkPageBuilder
{
  protected $m_node = null;
  protected $m_action = null;
  protected $m_record = null;
  
  protected $m_title = null;
  protected $m_boxes = array();

  /**
   * Constructor.
   *
   * @param atkNode $node
   */
  public function __construct(atkNode $node)
  {
    $this->m_node = $node;
    $this->m_action = $node->m_action;
  }
  
  /**
   * Returns the node.
   *
   * @return atkNode
   */
  public function getNode()
  {
    return $this->m_node;
  }
  
  /**
   * Sets the action.
   * 
   * @param string $action
   * 
   * @return atkPageBuilder
   */
  public function action($action)
  {
    $this->m_action = $action;
    return $this;
  }
  
  /**
   * Sets the record (if applicable) for this action.
   * 
   * @param array $record
   * 
   * @return atkPageBuilder
   */
  public function record($record)
  {
    $this->m_record = $record;
    return $this;
  }
  
  /**
   * Sets the page title to the given string.
   *
   * @param string $title
   * 
   * @return atkPage
   */
  public function title($title)
  {
    $this->m_title = $title;
    return $this;
  }
  
  /**
   * Add box.
   *
   * @param string $content
   * @param string $title
   * 
   * @return atkPageBuilder
   */
  public function box($content, $title=null)
  {
    $this->m_boxes[] = array('type' => 'box', 'title' => $title, 'content' => $content);
    return $this;
  }
  
  /**
   * Add action box.
   *
   * @param array  $params
   * @param string $title 
   * 
   * @return atkPageBuilder
   */
  public function actionBox($params, $title=null)
  {
    $this->m_boxes[] = array('type' => 'action', 'title' => $title, 'params' => $params);
    return $this;
  }  
  
  /**
   * Begins building a new action box.
   *
   * @return atkActionBoxBuilder
   */
  public function beginActionBox()
  {
    atkimport('atk.ui.atkactionboxbuilder');
    return new atkActionBoxBuilder($this);
  }
  
  /**
   * Renders the page.
   */
  public function render()
  {
    if ($this->m_title == null)
    {
      $this->m_title = $this->getNode()->actionTitle($this->m_action, $this->m_record);
    }    
    
    $boxes = array();
    foreach ($this->m_boxes as $box)
    {
      $title = $box['title'];      
      if ($title == null)
      {
        $title = $this->m_title;
      }
      
      if ($box['type'] == 'action')
      {
        $content = $this->getNode()->getUi()->renderAction($this->m_action, $box['params'], $this->getNode()->getModule());
      }
      else 
      {
        $content = $box['content'];
      }
      
      $boxes[] = $this->getNode()->getUi()->renderBox(array('title' => $title, 'content' => $content));
    }

    $this->getNode()->getPage()->setTitle(atktext('app_shorttitle')." - ".$this->m_title);
    
    $content = $this->getNode()->renderActionPage($this->m_title, $boxes);

    $this->getNode()->addStyle('style.css?v=1');      
    $this->getNode()->getPage()->addContent($content);
    return null;
  }
}
