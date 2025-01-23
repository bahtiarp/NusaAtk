<?php /* Smarty version 2.6.31, created on 2024-10-23 11:54:58
         compiled from ./atk/themes/default/templates/dgindex.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('modifier', 'escape', './atk/themes/default/templates/dgindex.tpl', 9, false),)), $this); ?>
<?php $_from = $this->_tpl_vars['links']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['i'] => $this->_tpl_vars['link']):
?>
  <?php if ($this->_tpl_vars['i'] > 0): ?>
    |
  <?php endif; ?>
  
  <?php if ($this->_tpl_vars['link']['current']): ?>
    <span class="bagde badge-danger"><?php echo $this->_tpl_vars['link']['title']; ?>
</span>
  <?php else: ?>
    <span href="javascript:void(0)" style="cursor: pointer;" class="badge text-bg-primary" onclick="<?php echo ((is_array($_tmp=$this->_tpl_vars['link']['call'])) ? $this->_run_mod_handler('escape', true, $_tmp) : smarty_modifier_escape($_tmp)); ?>
" title="<?php echo ((is_array($_tmp=$this->_tpl_vars['link']['title'])) ? $this->_run_mod_handler('escape', true, $_tmp) : smarty_modifier_escape($_tmp)); ?>
"><?php echo ((is_array($_tmp=$this->_tpl_vars['link']['title'])) ? $this->_run_mod_handler('escape', true, $_tmp) : smarty_modifier_escape($_tmp)); ?>
</span>
  <?php endif; ?>
<?php endforeach; endif; unset($_from); ?>