// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string_view>

namespace impeller {
namespace compiler {

constexpr std::string_view kReflectionHeaderTemplate =
    R"~~(// THIS FILE IS GENERATED BY impellerc.
// DO NOT EDIT OR CHECK THIS INTO SOURCE CONTROL

#pragma once

#include "shader_types.h"

namespace impeller {
namespace shader {

struct {{camel_case(shader_name)}}{{camel_case(shader_stage)}}Info {
  // ===========================================================================
  // Stage Info ================================================================
  // ===========================================================================
  static constexpr std::string_view kLabel = "{{camel_case(shader_name)}}";
  static constexpr std::string_view kEntrypointName = "{{entrypoint}}";
  static constexpr ShaderStage kShaderStage = {{to_shader_stage(shader_stage)}};


  // ===========================================================================
  // Struct Definitions ========================================================
  // ===========================================================================
{% for def in struct_definitions %}

  struct alignas(16) {{def.name}} {
{% for member in def.members %}
    {{member.type}} {{member.name}};
{% endfor %}
  }; // struct {{def.name}}
{% endfor %}

  // ===========================================================================
  // Stage Uniforms ============================================================
  // ===========================================================================
{% for uniform in uniform_buffers %}

  static constexpr auto kUniform{{camel_case(uniform.name)}} = ShaderStageIOSlot { // {{uniform.name}}
    "{{uniform.name}}",             // name
    {{uniform.location}}u,          // attribute location
    {{uniform.type.type_name}},     // type
    {{uniform.type.bit_width}}u,    // bit width of type
    {{uniform.type.vec_size}}u,     // vec size
    {{uniform.type.columns}}u       // number of columns
  };
{% endfor %}

  // ===========================================================================
  // Stage Inputs ==============================================================
  // ===========================================================================
{% for stage_input in stage_inputs %}

  static constexpr auto kInput{{camel_case(stage_input.name)}} = ShaderStageIOSlot { // {{stage_input.name}}
    "{{stage_input.name}}",             // name
    {{stage_input.location}}u,          // attribute location
    {{stage_input.type.type_name}},     // type
    {{stage_input.type.bit_width}}u,    // bit width of type
    {{stage_input.type.vec_size}}u,     // vec size
    {{stage_input.type.columns}}u       // number of columns
  };
{% endfor %}

  static constexpr std::array<const ShaderStageIOSlot*, {{length(stage_inputs)}}> kAllShaderStageInputs = {
{% for stage_input in stage_inputs %}
    &kInput{{camel_case(stage_input.name)}}, // {{stage_input.name}}
{% endfor %}
  };

  // ===========================================================================
  // Stage Outputs =============================================================
  // ===========================================================================
{% for stage_output in stage_outputs %}
  static constexpr auto kOutput{{camel_case(stage_output.name)}} = ShaderStageIOSlot { // {{stage_output.name}}
    "{{stage_output.name}}",             // name
    {{stage_output.location}}u,          // attribute location
    {{stage_output.type.type_name}},     // type
    {{stage_output.type.bit_width}}u,    // bit width of type
    {{stage_output.type.vec_size}}u,     // vec size
    {{stage_output.type.columns}}u       // number of columns
  };
{% endfor %}

  static constexpr std::array<const ShaderStageIOSlot*, {{length(stage_outputs)}}> kAllShaderStageOutputs = {
{% for stage_output in stage_outputs %}
    &kOutput{{camel_case(stage_output.name)}}, // {{stage_output.name}}
{% endfor %}
  };

}; // struct {{camel_case(shader_name)}}{{camel_case(shader_stage)}}Info

} // namespace shader
} // namespace impeller
)~~";

constexpr std::string_view kReflectionCCTemplate =
    R"~~(// THIS FILE IS GENERATED BY impellerc.
// DO NOT EDIT OR CHECK THIS INTO SOURCE CONTROL

#include "{{header_file_name}}"

#include <type_traits>

namespace impeller {
namespace shader {

using Info = {{camel_case(shader_name)}}{{camel_case(shader_stage)}}Info;

{% for def in struct_definitions %}
// Sanity checks for {{def.name}}
static_assert(std::is_standard_layout_v<Info::{{def.name}}>);
static_assert(sizeof(Info::{{def.name}}) == {{def.byte_length}});
{% for member in def.members %}
static_assert(offsetof(Info::{{def.name}}, {{member.name}}) == {{member.offset}});
{% endfor %}
{% endfor %}


} // namespace shader
} // namespace impeller
)~~";

}  // namespace compiler
}  // namespace impeller
