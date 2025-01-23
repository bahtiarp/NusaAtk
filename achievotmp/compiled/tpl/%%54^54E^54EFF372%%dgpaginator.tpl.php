<?php /* Smarty version 2.6.31, created on 2024-11-14 05:15:16
         compiled from ./themes/nusaatk/templates/dgpaginator.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('modifier', 'escape', './themes/nusaatk/templates/dgpaginator.tpl', 6, false),)), $this); ?>
<?php if ($this->_tpl_vars['links']): ?>
<ul class="pagination">
<?php endif; ?>
<?php $_from = $this->_tpl_vars['links']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['i'] => $this->_tpl_vars['link']):
?>
  <?php if ($this->_tpl_vars['link']['current']): ?>
    <li class="active"><a><?php echo ((is_array($_tmp=$this->_tpl_vars['link']['title'])) ? $this->_run_mod_handler('escape', true, $_tmp) : smarty_modifier_escape($_tmp)); ?>
 <span class="sr-only">(current)</span></a></li>
  <?php else: ?>
  		<?php if ($this->_tpl_vars['link']['title'] == 'Previous'): ?>
	  	<li><a href="javascript:void(0)" onclick="<?php echo ((is_array($_tmp=$this->_tpl_vars['link']['call'])) ? $this->_run_mod_handler('escape', true, $_tmp) : smarty_modifier_escape($_tmp)); ?>
" title="<?php echo ((is_array($_tmp=$this->_tpl_vars['link']['title'])) ? $this->_run_mod_handler('escape', true, $_tmp) : smarty_modifier_escape($_tmp)); ?>
">&laquo;</a></li>
	  	<?php elseif ($this->_tpl_vars['link']['title'] == 'Next'): ?>
	  	<li><a href="javascript:void(0)" onclick="<?php echo ((is_array($_tmp=$this->_tpl_vars['link']['call'])) ? $this->_run_mod_handler('escape', true, $_tmp) : smarty_modifier_escape($_tmp)); ?>
" title="<?php echo ((is_array($_tmp=$this->_tpl_vars['link']['title'])) ? $this->_run_mod_handler('escape', true, $_tmp) : smarty_modifier_escape($_tmp)); ?>
">&raquo;</a></li>
	  	<?php else: ?>
	  	<li><a href="javascript:void(0)" onclick="<?php echo ((is_array($_tmp=$this->_tpl_vars['link']['call'])) ? $this->_run_mod_handler('escape', true, $_tmp) : smarty_modifier_escape($_tmp)); ?>
" title="<?php echo ((is_array($_tmp=$this->_tpl_vars['link']['title'])) ? $this->_run_mod_handler('escape', true, $_tmp) : smarty_modifier_escape($_tmp)); ?>
"><?php echo ((is_array($_tmp=$this->_tpl_vars['link']['title'])) ? $this->_run_mod_handler('escape', true, $_tmp) : smarty_modifier_escape($_tmp)); ?>
</a></li>
	  <?php endif; ?>
  <?php endif; ?>
<?php endforeach; endif; unset($_from); ?>
<?php if ($this->_tpl_vars['links']): ?>
</ul>
<?php endif; ?>