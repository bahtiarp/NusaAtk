<?php /* Smarty version 2.6.31, created on 2024-10-23 11:54:44
         compiled from ./themes/nusaatk/templates/action.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'atkmessages', './themes/nusaatk/templates/action.tpl', 7, false),)), $this); ?>
    <div class="card card-secondary mb-4">

            <?php echo $this->_tpl_vars['formstart']; ?>


            <?php if (isset ( $this->_tpl_vars['helplink'] )): ?><div class="card-header"><div align="right" class="col-md-12 helplink"><?php echo $this->_tpl_vars['helplink']; ?>
</div></div><?php endif; ?>

            <?php echo smarty_function_atkmessages(array(), $this);?>

            <?php if (count ( $this->_tpl_vars['atkmessages'] )): ?>

                <div class="col-md-12"> 
                  
                        <?php $_from = $this->_tpl_vars['atkmessages']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['message']):
?>
                            <div  style="position:fixed;top:90px;" class="atkmessages"><?php echo $this->_tpl_vars['message']['message']; ?>
</div>
                        <?php endforeach; endif; unset($_from); ?>
                    
                    <script>fade("atkmessages");</script>
                </div>


            <?php endif; ?>  

            <?php if (( isset ( $this->_tpl_vars['header'] ) && ! empty ( $this->_tpl_vars['header'] ) )): ?>
                <div class="card-header"><?php echo $this->_tpl_vars['header']; ?>
</div>
            <?php endif; ?>
            <div class="card-body">
			
            <?php echo $this->_tpl_vars['content']; ?>

            </div>
            <div class="card-footer">
                <?php $_from = $this->_tpl_vars['buttons']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['button']):
?>
                    &nbsp;<?php echo $this->_tpl_vars['button']; ?>
&nbsp;
                <?php endforeach; endif; unset($_from); ?></div>

            <!-- div box -->
            <?php echo $this->_tpl_vars['formend']; ?>

  
    </div>