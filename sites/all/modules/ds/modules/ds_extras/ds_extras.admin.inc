<?php

/**
 * @file
 * Display Suite Extras administrative functions.
 */

/**
 * Menu callback: Display Suite extras settings.
 */
function ds_extras_settings($form) {

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#theme_wrappers' => array('vertical_tabs'),
    '#prefix' => '<div>',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );

  $form['additional_settings']['fs1'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field Templates'),
  );

  $form['additional_settings']['fs1']['ds_extras_field_template'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Field Templates'),
    '#description' => t('Customize the labels and the HTML output of your fields.'),
    '#default_value' => variable_get('ds_extras_field_template', FALSE),
  );

  $theme_functions = module_invoke_all('ds_field_theme_functions_info');
  unset($theme_functions['theme_ds_field_expert']);
  $form['additional_settings']['fs1']['ft-default'] = array(
    '#type' => 'select',
    '#title' => t('Default Field Template'),
    '#options' => $theme_functions,
    '#default_value' => variable_get('ft-default', 'theme_field'),
    '#description' => t('You can override this per field on the "Manage display" screens.<br />Default will output the field as defined in Drupal Core.<br />Reset will strip all HTML.<br />There is also an Expert Field Template, which you can only select per field, to have full control over the HTML.<br /><br /><label>Styles</label>You can add custom styles on the <a href="!url">styles form</a>. These styles can be added to fields using the Default Field Template.<br /><br /><label>Advanced</label>You can create your own custom field templates which need to be defined with hook_ds_field_theme_functions_info(). See ds.api.php for an example.', array('!url' => url('admin/structure/ds/styles'))),
    '#states' => array(
      'visible' => array(
        'input[name="additional_settings[fs1][ds_extras_field_template]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['additional_settings']['fs1']['ft-kill'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove theme suggestions'),
    '#default_value' => variable_get('ft-kill', FALSE),
    '#description' => t('Remove all theme suggestions from fields. If unsure, leave disabled.'),
    '#states' => array(
      'visible' => array(
        'input[name="additional_settings[fs1][ds_extras_field_template]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['additional_settings']['fs2'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extra fields'),
  );

  $form['additional_settings']['fs2']['ds_extras_fields_extra'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable extra fields'),
    '#description' => t('Make fields from other modules available on the "Manage display" screens.'),
    '#default_value' => variable_get('ds_extras_fields_extra', FALSE),
  );

  $form['additional_settings']['fs2']['ds_extras_fields_extra_list'] = array(
    '#type' => 'textarea',
    '#description' => t('Enter fields line by line, where each line is a combination of entity type, bundle and field name. E.g. node|article|tweetbutton. It might be possible that the actual content of the field depends on configuration of that field/module.'),
    '#default_value' => variable_get('ds_extras_fields_extra_list', FALSE),
    '#states' => array(
      'visible' => array(
        'input[name="additional_settings[fs2][ds_extras_fields_extra]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['additional_settings']['fs3'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other'),
  );

  $form['additional_settings']['fs3']['ds_extras_switch_view_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Switch view mode'),
    '#description' => t('Change view modes for individual nodes. A new tab "Display settings" will appear on the content create form.'),
    '#default_value' => variable_get('ds_extras_switch_view_mode', FALSE),
  );

  $form['additional_settings']['fs3']['ds_extras_switch_field'] = array(
    '#type' => 'checkbox',
    '#title' => t('Switch view mode field'),
    '#description' => t('Adds a field with links to switch view modes inline with Ajax. Only works for nodes at this time.'),
    '#default_value' => variable_get('ds_extras_switch_field', FALSE),
  );

  if (module_exists('contextual') && module_exists('field_ui')) {
    $form['additional_settings']['fs3']['ds_extras_contextual'] = array(
      '#type' => 'checkbox',
      '#title' => t('Contextual links'),
      '#description' => t('Add contextual links (if possible) to manage the layout.'),
      '#default_value' => variable_get('ds_extras_contextual', FALSE),
    );
  }

  $form['additional_settings']['fs3']['ds_extras_hide_page_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide page title'),
    '#description' => t('An extra option will appear on the "Manage displays" screen of the "Full content" build mode.'),
    '#default_value' => variable_get('ds_extras_hide_page_title', FALSE),
  );

  if (module_exists('block')) {
    $form['additional_settings']['fs3']['ds_extras_region_to_block'] = array(
      '#type' => 'checkbox',
      '#title' => t('Region to block'),
      '#description' => t('Create additional regions exposed as block. Note: this will not work on the default view mode.'),
      '#default_value' => variable_get('ds_extras_region_to_block', FALSE),
    );
  }

  if (module_exists('views')) {
    $form['additional_settings']['fs3']['ds_extras_vd'] = array(
      '#type' => 'checkbox',
      '#title' => t('Views displays'),
      '#description' => t('Manage the layout of your Views through a drag and drop interface at !url.', array('!url' => l(url('admin/structure/ds'), 'admin/structure/ds', array('absolute' => TRUE)))),
      '#default_value' => variable_get('ds_extras_vd', FALSE),
    );
  }

  if (module_exists('fivestar')) {
    $form['additional_settings']['fs3']['ds_extras_fivestar'] = array(
      '#type' => 'checkbox',
      '#title' => t('Fivestar field'),
      '#default_value' => variable_get('ds_extras_fivestar', FALSE),
    );
  }

  $form['#attached']['js'][] = drupal_get_path('module', 'ds_extras') . '/ds_extras.admin.js';

  $form = system_settings_form($form);
  $form['#submit'][] = 'ds_extras_settings_submit';
  return $form;
}

/**
 * Validate callback: Extras settings screen.
 */
function ds_extras_settings_validate($form, &$form_state) {
  foreach ($form_state['values']['additional_settings'] as $tab => $value) {
    if (is_array($value)) {
      foreach ($value as $variable => $val) {
        $form_state['values'][$variable] = $val;
      }
    }
    unset($form_state['values']['additional_settings'][$tab]);
  }
  unset($form_state['values']['additional_settings']);
}

/**
 * Submit callback: Extras settings screen.
 */
function ds_extras_settings_submit($form, &$form_state) {
  cache_clear_all('ds_fields', 'cache');
  cache_clear_all('entity_info:', 'cache', TRUE);
  cache_clear_all('theme_registry:', 'cache', TRUE);
  cache_clear_all('module_implements', 'cache_bootstrap');
  menu_rebuild();
}

/**
 * Alter Manage display screen.
 */
function ds_extras_field_ui_alter(&$form, &$form_state) {

  // Field Template.
  if (variable_get('ds_extras_field_template', FALSE) && isset($form['#ds_layout'])) {

    $field_settings = $form['#field_settings'];
    $field_styles = _ds_styles('ds_styles_fields');
    $functions = module_invoke_all('ds_field_theme_functions_info');
    $default_field_function = variable_get('ft-default', 'theme_field');

    $i = 1;
    foreach (element_children($form['fields']) as $key) {

      // Check if the label is overridden - add the original too.
      if (isset($form['fields'][$key]['human_name'])) {
        $form['fields'][$key]['human_name']['#prefix'] = '<div class="field-label-row"><input type="hidden" class="original-label" value="' . $form['fields'][$key]['human_name']['#markup'] . '">';
        $form['fields'][$key]['human_name']['#suffix'] = '</div>';
        if (isset($field_settings[$key]['ft']['lb'])) {
          $form['fields'][$key]['human_name']['#markup'] = t('@label (Original: @original)', array('@label' => $field_settings[$key]['ft']['lb'], '@original' => $form['fields'][$key]['human_name']['#markup']));
        }
      }

      // Link to open settings.
      $form['fields'][$key]['format']['ft'] = array(
        '#prefix' => '<a href="" class="ft-link">' . t('Field display') . '</a><div class="field-template" id="ft-' . $i . '">',
        '#suffix' => '</div>',
      );
      $i++;

      $field_function = isset($field_settings[$key]['ft']['func']) ? $field_settings[$key]['ft']['func'] : $default_field_function;

      // Functions.
      $form['fields'][$key]['format']['ft']['function'] = array(
        '#title' => t('Choose a Field Template'),
        '#type' => 'select',
        '#options' => $functions,
        '#default_value' => $field_function,
        '#attributes' => array(
          'class' => array('ds-extras-field-template'),
        ),
      );

      // Field styles.
      if (!empty($field_styles)) {
        $field_styles_select = array(
          '#type' => 'select',
          '#multiple' => TRUE,
          '#options' => $field_styles,
          '#title' => t('Choose a field style'),
          '#default_value' => isset($field_settings[$key]['ft']['styles']) ? explode(' ', $field_settings[$key]['ft']['styles']) : array(),
          '#prefix' => '<div class="field-styles">',
          '#suffix' => '</div>',
        );
        $form['fields'][$key]['format']['ft']['styles'] = $field_styles_select;
      }
      else {
        $form['fields'][$key]['format']['ft']['styles'] = array(
          '#type' => 'value',
          '#value' => array(''),
        );
      }

      // Wrappers and label.
      $wrappers = array(
        'lb' => array(
          'title' => t('Label'),
            'states' => array(
            'invisible' => array(
              'select[name="fields[' . $key . '][label]"]' => array('value' => 'hidden'),
              'select[name="fields[' . $key . '][format][ft][function]"]' => array('value' => 'theme_ds_field_expert'),
            ),
          ),
        ),
        'ow' => array('title' => t('Outer wrapper')),
        'fis' => array('title' => t('Field items')),
        'fi' => array('title' => t('Field item')),
      );
      foreach ($wrappers as $wrapper_key => $value) {
        $form['fields'][$key]['format']['ft'][$wrapper_key] = array(
          '#type' => 'checkbox',
          '#title' => $value['title'],
          '#prefix' => '<div class="ft-group ' . $wrapper_key . '">',
          '#default_value' => isset($field_settings[$key]['ft'][$wrapper_key]) ? TRUE : FALSE,
        );
        if (isset($value['states'])) {
          $form['fields'][$key]['format']['ft'][$wrapper_key]['#states'] = $value['states'];
        }
        $form['fields'][$key]['format']['ft'][$wrapper_key . '-el'] = array(
          '#type' => 'textfield',
          '#title' => t('Element'),
          '#size' => '10',
          '#description' => t('E.g. div, span, h2 etc.'),
          '#default_value' => isset($field_settings[$key]['ft'][$wrapper_key . '-el']) ? $field_settings[$key]['ft'][$wrapper_key . '-el'] : '',
        );
        if (isset($value['states']) && $wrapper_key == 'lb') {
          $form['fields'][$key]['format']['ft'][$wrapper_key . '-el']['#states'] = $value['states'];
        }
        $classes = array(
          'field-name-' . strtr($key, '_', '-'),
        );
        $form['fields'][$key]['format']['ft'][$wrapper_key . '-cl'] = array(
          '#type' => 'textfield',
          '#title' => t('Classes'),
          '#size' => '10',
          '#suffix' => '</div><div class="clearfix"></div>',
          '#default_value' => isset($field_settings[$key]['ft'][$wrapper_key . '-cl']) ? $field_settings[$key]['ft'][$wrapper_key . '-cl'] : '',
          '#description' => t('Classes: !classes', array('!classes' => implode(', ', $classes))),
        );
        if (isset($value['states'])) {
          $form['fields'][$key]['format']['ft'][$wrapper_key . '-cl']['#states'] = $value['states'];
        }
      }

      $form['fields'][$key]['format']['ft']['update'] = array(
        '#type' => 'button',
        '#value' => t('Update'),
        '#attributes' => array(
          'class' => array('ft-update'),
        ),
      );

      // Another label needs some other stuff.
      unset($form['fields'][$key]['format']['ft']['lb']['#description']);
      $form['fields'][$key]['format']['ft']['lb']['#states'] = array(
        'invisible' => array(
          'select[name="fields[' . $key . '][label]"]' => array('value' => 'hidden'),
        )
      );
      $form['fields'][$key]['format']['ft']['lb']['#type'] = 'textfield';
      $form['fields'][$key]['format']['ft']['lb']['#size'] = '10';
      $form['fields'][$key]['format']['ft']['lb']['#attributes'] = array('class' => array('label-change'));
      $form['fields'][$key]['format']['ft']['lb']['#default_value'] = isset($field_settings[$key]['ft']['lb']) ? $field_settings[$key]['ft']['lb'] : '';
    }
  }

  // Views displays.
  if (variable_get('ds_extras_vd', FALSE)) {
    // Add an additional submit callback.
    $form['#submit'] = array_merge(array('ds_extras_vd_field_ui_submit'), $form['#submit']);
  }

  // Hide page title functionality, currently only works on node.
  if (isset($form['#ds_layout']) && $form['#entity_type'] == 'node' && ($form['#view_mode'] == 'full' || variable_get('ds_extras_switch_view_mode', FALSE)) &&  variable_get('ds_extras_hide_page_title', FALSE)) {
    $form['additional_settings']['ds_layouts']['ds_hide_page_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide page title'),
      '#default_value' => isset($form['#ds_layout']->settings['hide_page_title']) ? $form['#ds_layout']->settings['hide_page_title'] : FALSE,
      '#weight' => 100,
    );
  }

  // Region to block only fires if there is a layout and we're working on the
  // a view mode which is not equal to default.
  if (isset($form['#ds_layout']) && $form['#view_mode'] != 'default' && variable_get('ds_extras_region_to_block', FALSE)) {

    $layout = $form['#ds_layout'];

    // Get the entity_type, bundle and view mode.
    $entity_type = $form['#entity_type'];
    $bundle = $form['#bundle'];
    $view_mode = $form['#view_mode'];

    $region_blocks_options = array();
    $region_blocks = variable_get('ds_extras_region_blocks', array());
    foreach ($region_blocks as $key => $block) {
      if ($block['info'] == "{$entity_type}_{$bundle}_{$view_mode}") {
        $region_blocks_options[$key] = t('Remove') . ' ' . $block['title'];
      }
    }

    $form['additional_settings']['region_to_block'] = array(
      '#type' => 'fieldset',
      '#title' => t('Block regions'),
      '#description' => t('Create additional regions in this layout which will be exposed as blocks.')
    );

    $form['additional_settings']['region_to_block']['new_block_region'] = array(
      '#type' => 'textfield',
      '#title' => t('Region name'),
      '#description' => t('Enter a name to create a new region.'),
    );
    $form['additional_settings']['region_to_block']['new_block_region_key'] = array(
      '#title' => t('Machine name'),
      '#type' => 'machine_name',
      '#default_value' => '',
      '#maxlength' => 32,
      '#required' => FALSE,
      '#description' => t('The machine-readable name of this block region. This name must contain only lowercase letters and underscores. This name must be unique.'),
      '#disabled' => FALSE,
      '#machine_name' => array(
        'exists' => 'ds_extras_region_to_block_unique',
        'source' => array('additional_settings', 'region_to_block', 'new_block_region'),
      ),
    );

    if (!empty($region_blocks_options)) {
      $form['additional_settings']['region_to_block']['remove_block_region'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Existing block regions'),
        '#options' => $region_blocks_options,
        '#description' => t('Check the regions you want to remove.'),
      );
    }

    $form['#submit'][] = 'ds_extras_block_submit';
  }
}

/**
 * Implements hook_ds_field_format_summary().
 */
function ds_extras_ds_field_format_summary($field) {
  if (isset($field['formatter_settings'])) {
    foreach ($field['formatter_settings'] as $key => $value) {
      if (!empty($value)) {
        return t('Configured');
        break;
      }
    }
  }
  return t('Not configured');
}

/**
 * Implements hook_ds_field_settings_form().
 */
function ds_extras_ds_field_settings_form($field) {
  $form = array();

  $entity_type = $field['entity_type'];
  $bundle = $field['bundle'];
  $view_mode = $field['view_mode'];
  $settings = isset($field['formatter_settings']) ? $field['formatter_settings'] : array();
  $view_modes = ds_entity_view_modes($entity_type);

  $form['info'] = array(
    '#markup' => t('Enter a label for the link for the view modes you want to switch to.<br />Leave empty to hide link. They will be localized.'),
  );

  foreach ($view_modes as $key => $value) {

    $view_mode_settings = field_view_mode_settings($entity_type, $bundle);
    $visible = !empty($view_mode_settings[$key]['custom_settings']);

    if ($visible) {
      $form[$key] = array(
        '#type' => 'textfield',
        '#default_value' => isset($settings[$key]) ? $settings[$key] : '',
        '#size' => 20,
        '#title' => check_plain($value['label']),
      );
    }
  }

  return $form;
}

/**
 * Implements hook_ds_field_settings_alter().
 */
function ds_extras_ds_field_settings_alter(&$field_settings, $form, &$form_state) {

  $fields = $form_state['values']['fields'];
  $default_field_function = variable_get('ft-default', 'theme_field');
  $wrappers = array(
    'ow' => t('Wrapper'),
    'fis' => t('Field items'),
    'fi' => t('Field item')
  );

  foreach ($fields as $key => $field) {

    // Make sure we need to save anything for this field.
    if (_ds_field_valid($key, $field, $form_state)) {
      continue;
    }

    // Get the values.
    $values = isset($fields[$key]['format']['ft']) ? $fields[$key]['format']['ft'] : array();
    if (empty($values)) {
      continue;
    }

    // Theme output function.
    $function = $values['function'];
    if ($function != $default_field_function) {
      $field_settings[$key]['ft']['func'] = $function;
    }

    // Field styles.
    if ($function != 'theme_ds_field_expert' && $function != 'theme_ds_field_reset') {
      $classes = implode(' ', $values['styles']);
      if (!empty($classes)) {
        $field_settings[$key]['ft']['styles'] = $classes;
      }
    }

    // Label.
    if (isset($fields[$key]['label']) && $fields[$key]['label'] != 'hidden') {
      if (!empty($values['lb'])) {
        $field_settings[$key]['ft']['lb'] = $values['lb'];
      }
      if (!(empty($values['lb-el'])) && $function == 'theme_ds_field_expert') {
        $field_settings[$key]['ft']['lb-el'] = check_plain($values['lb-el']);
      }
      if (!(empty($values['lb-cl'])) && $function == 'theme_ds_field_expert') {
        $field_settings[$key]['ft']['lb-cl'] = check_plain($values['lb-cl']);
      }
    }

    // Custom field configuration.
    if ($function == 'theme_ds_field_expert') {
      foreach ($wrappers as $wrapper_key => $title) {
        if ($values[$wrapper_key]) {
          // Enable.
          $field_settings[$key]['ft'][$wrapper_key] = TRUE;
          // Element.
          $field_settings[$key]['ft'][$wrapper_key . '-el'] = !(empty($values[$wrapper_key . '-el'])) ? check_plain($values[$wrapper_key . '-el']) : 'div';
          // Classess.
          $field_settings[$key]['ft'][$wrapper_key . '-cl'] = !(empty($values[$wrapper_key . '-cl'])) ? check_plain($values[$wrapper_key . '-cl']) : '';
        }
      }
    }
  }
}

/**
 * Implements hook_ds_layout_settings_alter().
 */
function ds_extras_ds_layout_settings_alter($record, $form_state) {
  if (isset($form_state['values']['additional_settings']['ds_hide_page_title'])) {
    $record->settings['hide_page_title'] = $form_state['values']['additional_settings']['ds_hide_page_title'];
  }
}

/**
 * Submit callback after Field UI submission of a views display.
 */
function ds_extras_vd_field_ui_submit($form, &$form_state) {
  // Add the 'type' key to the extra title key so we can ditch the notice.
  $form_state['values']['fields']['title']['type'] = 'hidden';
}

/**
 * Submit callback: manage block regions.
 */
function ds_extras_block_submit($form, &$form_state) {

  // Create new region.
  if (!empty($form_state['values']['additional_settings']['region_to_block']['new_block_region'])) {

    // Get the entity_type, bundle and view mode.
    $entity_type = $form['#entity_type'];
    $bundle = $form['#bundle'];
    $view_mode = $form['#view_mode'];

    $block = array(
      'title' => $form_state['values']['additional_settings']['region_to_block']['new_block_region'],
      'info' => "{$entity_type}_{$bundle}_{$view_mode}",
    );

    $block_key = $form_state['values']['additional_settings']['region_to_block']['new_block_region_key'];
    $region_blocks = variable_get('ds_extras_region_blocks', array());
    $region_blocks[$block_key] = $block;
    variable_set('ds_extras_region_blocks', $region_blocks);
  }

  // Remove a region.
  if (isset($form_state['values']['additional_settings']['region_to_block']['remove_block_region'])) {
    $variable_set = FALSE;
    $region_blocks = variable_get('ds_extras_region_blocks', array());
    $remove = $form_state['values']['additional_settings']['region_to_block']['remove_block_region'];
    foreach ($remove as $key => $value) {
      if ($key === $value) {
        $variable_set = TRUE;
        db_delete('block')
          ->condition('delta', $key)
          ->condition('module', 'ds_extras')
          ->execute();
        unset($region_blocks[$key]);
      }
    }

    if ($variable_set) {
      variable_set('ds_extras_region_blocks', $region_blocks);
    }
  }
}

/**
 * Return unique region to block.
 */
function ds_extras_region_to_block_unique($name) {
  $region_blocks = variable_get('ds_extras_region_blocks', array());
  $value = strtr($name, array('-' => '_'));
  return isset($region_blocks[$value]) ? TRUE : FALSE;
}
