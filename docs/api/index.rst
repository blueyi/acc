.. C++ API (generated from include/ACC via Doxygen + Breathe)

C++ API Reference
=================

API documentation is generated from Doxygen comments in ``include/ACC``.
Build the docs with ``doxygen docs/Doxyfile`` then ``sphinx-build`` (or use the
top-level ``make -C docs html``).

Do not list the same type in both ``doxygennamespace`` and ``doxygenclass``,
or Sphinx will report duplicate IDs.

Selected types in namespace ``acc``
-----------------------------------

.. doxygenclass:: acc::ExprAST
   :members:
   :undoc-members:

.. doxygenclass:: acc::Parser
   :members:
   :undoc-members:

.. doxygenclass:: acc::Sema
   :members:
   :undoc-members:

.. doxygenstruct:: acc::TensorType
   :members:

.. doxygenclass:: acc::LayerDeclAST
   :members:
   :undoc-members:

.. doxygenclass:: acc::ModelDeclAST
   :members:
   :undoc-members:

.. doxygenclass:: acc::CompileDeclAST
   :members:
   :undoc-members:

.. doxygenclass:: acc::ModuleAST
   :members:
   :undoc-members:
