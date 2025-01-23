<?php /* Smarty version 2.6.31, created on 2024-11-14 05:15:16
         compiled from ./themes/nusaatk/templates/list.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'atkmessages', './themes/nusaatk/templates/list.tpl', 3, false),)), $this); ?>
<?php if (isset ( $this->_tpl_vars['formstart'] )): ?><?php echo $this->_tpl_vars['formstart']; ?>
<?php endif; ?>

  <?php echo smarty_function_atkmessages(array(), $this);?>

  <?php if (count ( $this->_tpl_vars['atkmessages'] )): ?>

 
           <div style='position:fixed;left:40%;top:150px;z-index:9999;margin-right:10px;' class="atkmessages">
          <?php $_from = $this->_tpl_vars['atkmessages']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['message']):
?>
            <div class="atkmessages"><?php echo $this->_tpl_vars['message']['message']; ?>
</div>
          <?php endforeach; endif; unset($_from); ?>             
    
        </div>
        <script>fade("atkmessages");</script>
      
  <?php endif; ?>  
  <?php if (( isset ( $this->_tpl_vars['header'] ) && ! empty ( $this->_tpl_vars['header'] ) )): ?>
<div class="list-header">
   <?php echo $this->_tpl_vars['header']; ?>

  </div>
  <?php endif; ?>
  <!--
  <?php if (( isset ( $this->_tpl_vars['index'] ) && ! empty ( $this->_tpl_vars['index'] ) )): ?>
  <div>
    <span valign="top" align="left"><?php echo $this->_tpl_vars['index']; ?>
<br><br></span>
  </div>
  <?php endif; ?>
  -->
  <?php if (( isset ( $this->_tpl_vars['navbar'] ) && ! empty ( $this->_tpl_vars['navbar'] ) )): ?>
  <div>
    <span valign="top" align="left"><?php echo $this->_tpl_vars['navbar']; ?>
<br></span>
  </div>
  <?php endif; ?>
  <div class="noname">
    <span valign="top" align="left">
        <?php echo $this->_tpl_vars['list']; ?>

    </span>
  </div>
  <?php if (( isset ( $this->_tpl_vars['navbar'] ) && ! empty ( $this->_tpl_vars['navbar'] ) )): ?>
  <div>
    <span valign="top" align="left"><?php echo $this->_tpl_vars['navbar']; ?>
<br></span>
  </div>
  <?php endif; ?>
  <?php if (( isset ( $this->_tpl_vars['footer'] ) && ! empty ( $this->_tpl_vars['footer'] ) )): ?>
  <div>
    <span valign="top" align="left"><?php echo $this->_tpl_vars['footer']; ?>
<br></span>
  </div>
  <?php endif; ?>

<?php if (isset ( $this->_tpl_vars['formstart'] )): ?><?php echo $this->_tpl_vars['formend']; ?>
<?php endif; ?>